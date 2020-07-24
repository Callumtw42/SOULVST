import React, { Component } from 'react';
import { clamp } from "../functions"
import {
  Image,
  View,
  Canvas
} from 'juce-blueprint';
import Mouse from "./mouse"
import NodeList from "./nodelist"
import Slider from "./slider"
import Dial from "./dial"

const boxHeight = 100;
const boxWidth = 150;
const plotHeight = (3 / 4) * boxHeight;
const plotWidth = boxWidth;
const pointRadius = 4;
const plotResolution = 128;
const MAX_GRID_RES = 16;

const GRID_COLOR = "rgba(122, 129, 132, 0.25)"

class LFO extends Component {
  constructor(props) {
    super(props);

    this._onMouseDown = this._onMouseDown.bind(this);
    this._onMouseDrag = this._onMouseDrag.bind(this);
    this._onMouseUp = this._onMouseUp.bind(this);
    this._onMouseDoubleClick = this._onMouseDoubleClick.bind(this);
    this.initialised = false;

    this.state = {
      points: new NodeList(
        {
          x: 0,
          y: 0,
          radius: pointRadius,
          isSelected: false,
          isBound: true,
          leftNeighbour: null,
          rightNeighbour: null
        },
        {
          x: plotWidth,
          y: 0,
          radius: pointRadius,
          isSelected: false,
          isBound: true,
          leftNeighbour: null,
          rightNeighbour: null
        }
      ),
      plot: new Array(plotResolution),
      gridRes: 16
    }
  }

  componentDidMount() {

    this.state.points.insertAfter(0,
      {
        x: plotWidth / 2,
        y: plotHeight,
        radius: pointRadius,
        isSelected: false,
        isBound: false,
        leftNeighbour: null,
        rightNeighbour: null
      },
    )
    this.setState(this.state);
  }

  _onMouseDoubleClick(mouseX, mouseY) {
    const { points } = this.state;

    let mouse = new Mouse(mouseX, mouseY);
    let removing = false;

    for (const point of points.get()) {
      if (mouse.isOverlapping(point) && !point.isBound) {
        const index = points.indexOf(point);
        points.remove(index);
        removing = true;
        break;
      }
    }

    if (!removing) {
      let leftNeighbour = points.get()[0];
      let shortestLeftPath = mouse.x - leftNeighbour.x;

      for (const point of points.get()) {
        let leftPath = mouse.x - point.x;
        if (leftPath < 0)
          break;
        else if (leftPath <= shortestLeftPath) {
          leftNeighbour = point;
        }
      }
      const index = points.indexOf(leftNeighbour);

      points.insertAfter(
        index,
        {
          x: mouseX,
          y: mouseY,
          radius: pointRadius,
          isSelected: false,
        }
      )
    }

    this.setState(this.state);
  }


  _onMouseDrag(mouseX, mouseY, mouseDownX, mouseDownY) {
    const { gridRes } = this.state
    const dx = plotWidth / gridRes
    const dy = plotHeight / gridRes
    const x = Math.round((mouseX / plotWidth) * gridRes) * dx
    const y = Math.round((mouseY / plotHeight) * gridRes) * dy

    this.state.points.forEach((point) => {
      if (point.isSelected) {
        point.y = clamp(y, 0, plotHeight);
        if (!point.isBound) {
          point.x = clamp(x, point.leftNeighbour.x, point.rightNeighbour.x);
        }
      }

    })

    this.setState(this.state);
  }

  _onMouseDown(mouseX, mouseY) {

    this.initialised = true;
    global.log(mouseX)
    const points = this.state.points;
    let mouse = new Mouse(mouseX, mouseY);
    points.forEach((point) => {
      if (mouse.isOverlapping(point)) {
        point.isSelected = true;
      }
    });
  }

  _onMouseUp(mouseX, mouseY) {
    this.state.points.forEach((point) => {
      if (point.isSelected) {
        point.isSelected = false;
      }
    })
  }

  generatePlot() {
    const { points, plot } = this.state;

    let startIndex = 0;
    points.forEach((point, index) => {

      if (point.rightNeighbour != null) {
        const rightNeighbourY = point.rightNeighbour.y / plotHeight;
        const rightNeighbourX = point.rightNeighbour.x / plotWidth;
        const x = point.x / plotWidth;
        const y = point.y / plotHeight;
        const targetIndex = Math.round(rightNeighbourX * plotResolution);
        const dy = (rightNeighbourY - y) / (rightNeighbourX - x) / plotResolution;
        let value = y;
        for (let i = startIndex; i < targetIndex; i++) {
          plot[i] = 1 - value;
          value += dy;
        }
        startIndex = targetIndex;
      }
    })
    if (this.initialised) global.sendPlot(this.props.paramId, plot);
  }

  grid() {
    const { gridRes } = this.state
    const verticalPaths = new Array(gridRes)
    const horizontalPaths = new Array(gridRes)
    let dx = plotWidth / gridRes
    let dy = plotHeight / gridRes
    for (let i = 0; i < gridRes; i++) {
      verticalPaths[i] = `<path d="M${dx * i} ${0} L${dx * i} ${plotHeight} Z" stroke="${GRID_COLOR}" stroke-width="1"/>`
      horizontalPaths[i] = `<path d="M${0} ${dy * i} L${plotWidth} ${dy * i} Z" stroke="${GRID_COLOR}" stroke-width="1"/>`
    }
    return {
      gridX: verticalPaths,
      gridY: horizontalPaths
    }
  }

  _svg() {
    const { points, plot } = this.state;
    const { gridX, gridY } = this.grid();
    this.generatePlot();
    const paths = points.map((point) => {
      if (point.rightNeighbour) return `<path d="M${point.x} ${point.y} L${point.rightNeighbour.x} ${point.rightNeighbour.y} Z" stroke="#3ade3a" stroke-width="2"/>`
      else return ``;
    })
    const circles = points.map((point, index) => {
      return `
      <circle cx="${point.x}" cy="${point.y}" r="${point.radius}" fill="#3ade3a" />
      ${paths[index]}
        `
    })

    const img =
      `
      <svg width="${boxWidth}" height="${boxWidth}" viewBox="0 0 0 0" xmlns="http://www.w3.org/2000/svg">
        <rect x="${0}" y="${0}" stroke="green" stroke-width="1" fill="#2a302a" width="${boxWidth}" height="${plotHeight}" />
        ${gridX}
        ${gridY}
        ${circles}
      </svg>
      `
    return img;
  }

  setSpeed(value) {
    global.sendLFOSpeed(this.props.paramId, value);
  }

  setGridRes(value) {
    // const newValue = clamp(Math.round(value * MAX_GRID_RES), 1, 16)
    this.setState({ gridRes: value })
  }

  render() {
    styles.container.left = this.props.x;
    styles.container.top = this.props.y;
    if (this.props.paramId == this.props.activeLFO) {
      return (
        <View {...styles.container}>
          <Image
            onMouseDown={this._onMouseDown}
            onMouseUp={this._onMouseUp}
            onMouseDrag={this._onMouseDrag}
            onMouseDoubleClick={this._onMouseDoubleClick}
            {...styles.plot} source={this._svg()} />
          {/* <Slider {...styles.slider} callBack={this.setSpeed.bind(this)}></Slider> */}
          {/* <Slider {...styles.slider} {...this.props} min={1} max={16} step={1} callBack={this.setGridRes.bind(this)} ></Slider> */}
          <Dial {...styles.dial} {...this.props} min={1} max={16} step={1} callBack={this.setGridRes.bind(this)} ></Dial>
        </View>
      );
    }
    else return <></>
  }
}

let styles = {
  plot: {
    'height': plotHeight,
    'width': plotWidth,
    // 'position': 'absolute',
    'interceptClickEvents': true,
  },
  slider: {
    'width': boxWidth,
    'height': boxHeight - plotHeight,
  },
  dial: {
    'width': boxWidth / 4,
    'height': boxWidth / 4,
    // 'transform-rotate': Math.PI * 1.25,
  },
  container: {
    'flex-direction': 'column',
    'position': 'absolute',
    'left': 0,
    'top': 0
  },
};

export { LFO, boxHeight as lfoHeight, boxWidth as lfoWidth, plotHeight as lfoViewHeight };
