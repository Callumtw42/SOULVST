import React, { Component } from 'react';
import { clamp } from "../functions"
import {
  Image,
  View,
  Canvas
} from 'juce-blueprint';
import Mouse from "./mouse"
import { Node, NodeList } from "./nodelist"
import Slider from "../slider/slider"
import Dial from "../slider/dial"
import Button from "../button"

const boxHeight = 100;
const boxWidth = 150;
export const plotHeight = (3 / 4) * boxHeight;
const plotWidth = boxWidth;
export const pointRadius = 4;
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
        new Node(0, 0, pointRadius, true),
        new Node(plotWidth, 0, pointRadius, true)
      ),
      plot: new Array(plotResolution),
      gridRes: 16,
      lineColor: "#626262",
      on: false,
      speed: 4.0
    }
  }

  componentDidMount() {

    this.state.points.insertAfter(0,
      new Node(plotWidth / 2, plotHeight, pointRadius, false)
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
        new Node(mouseX, mouseY, pointRadius, false)
      )
    }

    this.setState(this.state);
  }

  _onMouseDrag(mouseX, mouseY, mouseDownX, mouseDownY) {
    const { gridRes, points } = this.state
    const dx = plotWidth / gridRes
    const dy = plotHeight / gridRes
    const snapX = Math.round((mouseX / plotWidth) * gridRes) * dx
    const snapY = Math.round((mouseY / plotHeight) * gridRes) * dy

    for (const point of points.get()) {
      if (point.isSelected) {
        point.move(snapX, snapY)
        break;
      }
      const path = point.path;
      if (path && path.controlPoint.isSelected) {
        path.moveControlPoint(mouseY);
        break;
      }

    }
    this.setState(this.state);
  }

  _onMouseDown(mouseX, mouseY) {

    this.initialised = true;
    const points = this.state.points;
    let mouse = new Mouse(mouseX, mouseY);

    for (const point of points.get()) {
      if (mouse.isOverlapping(point)) {
        point.isSelected = true;
        break;
      }
      if (point.path && mouse.isOverlapping(point.path.controlPoint)) {
        point.path.controlPoint.isSelected = true;
        break;
      }
    }
  }

  _onMouseUp(mouseX, mouseY) {
    this.state.points.forEach((point) => {
      if (point.isSelected) {
        point.isSelected = false;
      }
      if (point.path)
        point.path.controlPoint.isSelected = false;
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
    const { points, plot, lineColor } = this.state;
    const { gridX, gridY } = this.grid();
    this.generatePlot();

    const paths = points.map((point) => {
      const path = point.path;
      if (path) {
        const control = path.controlPoint;
        const start = path.startNode;
        const end = path.endNode;
        const dy = end.y - start.y;
        const dx = end.x - start.x;
        const relY = control.relY;

        const D = Math.abs(dy);

        const ctrl1 = (relY >= 0)
          ? {
            x: start.x + dx * relY,
            y: start.y
          }
          : {
            x: start.x,
            y: start.y - dy * relY
          }

        const ctrl2 = (relY >= 0)
          ? {
            x: end.x,
            y: end.y - dy * relY
          }
          : {
            x: end.x + dx * relY,
            y: end.y
          }

        return `<path 
      d="M${start.x} ${start.y} 
      C${ctrl1.x} ${ctrl1.y} ${ctrl2.x} ${ctrl2.y} ${end.x} ${end.y}" 
      stroke="${lineColor}" stroke-width="2"/>
      <circle cx="${control.x}" cy="${control.y}" r="${control.radius}" stroke="${lineColor}" stroke-width="1" />
      <circle cx="${ctrl1.x}" cy="${ctrl1.y}" r="${1}" stroke="${"#ff00ff"}" stroke-width="1" />
      <circle cx="${ctrl2.x}" cy="${ctrl2.y}" r="${1}" stroke="${"#ffffff"}" stroke-width="1" />
      `
      }
      else return ``;
    })

    const circles = points.map((point, index) => {
      return `<circle cx="${point.x}" cy="${point.y}" r="${point.radius}" fill="${lineColor}" />
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
    this.setState({
      speed: value
    })
    global.sendLFOSpeed(this.props.paramId, value);
  }

  setGridRes(value) {
    this.setState({ gridRes: Math.round(value) })
  }

  setActive(buttonOn) {
    this.setState({
      lineColor: buttonOn ? "#3ade3a" : "#626262",
      on: buttonOn
    })
    global.setLFOEnabled(this.props.paramId, buttonOn)
    global.sendPlot(this.props.paramId, this.state.plot)
  }

  render() {
    const { speed, gridRes } = this.state;
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
          <View {...styles.dials}>
            <Dial {...styles.dial} value={gridRes} start={1} end={16} step={1} label={"Grid"} callBack={this.setGridRes.bind(this)} ></Dial>
            <Dial {...styles.dial} value={speed} start={4} end={1 / 64} step={-2} skew={0.5} label={"Speed"} callBack={this.setSpeed.bind(this)} ></Dial>
            <Button {...styles.button}{...this.props} callBack={this.setActive.bind(this)} on={this.state.on} />
          </View>
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
  dials: {
    'flex-direction': 'row'
  },
  container: {
    'flex-direction': 'column',
    'position': 'absolute',
    'left': 0,
    'top': 0
  },
  button: {
    'width': boxWidth / 4,
    'height': boxWidth / 4,
    'color': '#3ade3a',
  }
};

export { LFO, boxHeight as lfoHeight, boxWidth as lfoWidth, plotHeight as lfoViewHeight };
