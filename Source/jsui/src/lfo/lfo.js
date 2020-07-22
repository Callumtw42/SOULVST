import React, { Component } from 'react';
import { clamp } from "../functions"
import {
  Image,
  View
} from 'juce-blueprint';
import Mouse from "./mouse"
import NodeList from "./nodelist"
import Slider from "./slider"

const pointRadius = 4;
const viewHeight = 75;
const canvasHeight = 85;
const canvasWidth = 100;
const plotResolution = 128;

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
          x: canvasWidth,
          y: 0,
          radius: pointRadius,
          isSelected: false,
          isBound: true,
          leftNeighbour: null,
          rightNeighbour: null
        }
      ),
      plot: new Array(plotResolution)
    }
  }

  componentDidMount() {

    this.state.points.insertAfter(0,
      {
        x: canvasWidth / 2,
        y: viewHeight,
        radius: pointRadius,
        isSelected: false,
        isBound: false,
        leftNeighbour: null,
        rightNeighbour: null
      },
    )
    this.setState(this.state);
  }

  componentWillUnmount() {
  }

  log(s) {
    global.log();
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

    this.state.points.forEach((point) => {
      if (point.isSelected) {
        point.y = clamp(mouseY, 0, viewHeight);
        if (!point.isBound) {
          point.x = clamp(mouseX, point.leftNeighbour.x, point.rightNeighbour.x);
        }
      }

    })

    this.setState(this.state);
  }

  _onMouseDown(mouseX, mouseY) {

    this.initialised = true;

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
        const rightNeighbourY = point.rightNeighbour.y / viewHeight;
        const rightNeighbourX = point.rightNeighbour.x / canvasWidth;
        const x = point.x / canvasWidth;
        const y = point.y / viewHeight;
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

  _svg() {
    const { points, plot } = this.state;
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
      <svg width="${canvasWidth}" height="${viewHeight}" viewBox="0 0 0 0" xmlns="http://www.w3.org/2000/svg">
        <rect x="${0}" y="${0}" stroke="green" stroke-width="1" fill="#2a302a" width="${canvasWidth}" height="${viewHeight}" />
        ${circles}
      </svg>
      `
    return img;
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
            {...styles.canvas} source={this._svg()} />
          <Slider {...styles.slider} paramId={this.props.paramId}></Slider>
        </View>
      );
    }
    else return <></>
  }
}

let styles = {
  canvas: {
    'height': viewHeight,
    'width': canvasWidth,
    // 'position': 'absolute',
    'interceptClickEvents': true,
  },
  slider: {
    'width': canvasWidth,
    'height': canvasHeight - viewHeight,
  },
  container: {
    'flex-direction': 'column',
    'position': 'absolute',
    'left': 0,
    'top': 0
  },
};

export { LFO, canvasHeight as lfoHeight, canvasWidth as lfoWidth, viewHeight as lfoViewHeight };
