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
const CURVE_RES = 32;

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

  getControlPoints(start, end) {
    const dy = end.y - start.y;
    const dx = end.x - start.x;
    const relY = 1 - ((start.path.controlPoint.relY + 1) / 2);
    // global.log(relY)

    // const ctrl1 = (relY >= 0)
    //   ? {
    //     x: start.x + dx * relY,
    //     y: start.y
    //   }
    //   : {
    //     x: start.x,
    //     y: start.y - dy * relY
    //   }

    // const ctrl2 = (relY >= 0)
    //   ? {
    //     x: end.x,
    //     y: end.y - dy * relY
    //   }
    //   : {
    //     x: end.x + dx * relY,
    //     y: end.y
    //   }
    const ctrl1 = {
      x: end.x - dx * relY,
      y: start.y + dy * relY
    }

    const ctrl2 = {
      x: end.x - dx * relY,
      y: start.y + dy * relY
    }

    // global.log(relY);


    return { ctrl1, ctrl2 }
  }

  bezier(p0, p1, p2, p3, t) {
    var cX = 3 * (p1.x - p0.x),
      bX = 3 * (p2.x - p1.x) - cX,
      aX = p3.x - p0.x - cX - bX;

    var cY = 3 * (p1.y - p0.y),
      bY = 3 * (p2.y - p1.y) - cY,
      aY = p3.y - p0.y - cY - bY;

    var x = (aX * Math.pow(t, 3)) + (bX * Math.pow(t, 2)) + (cX * t) + p0.x;
    var y = (aY * Math.pow(t, 3)) + (bY * Math.pow(t, 2)) + (cY * t) + p0.y;

    return { x: x, y: y };
  }

  normalisePlot() {
    return this.state.plot.map((y, index) => {
      return 1 - y / plotHeight;
    })
  }
  generatePlot() {
    const { points, plot } = this.state;
    points.forEach((point, index) => {
      if (point.rightNeighbour) {
        const start = point;
        const end = point.rightNeighbour;
        const { ctrl1, ctrl2 } = this.getControlPoints(start, end);
        const startIndex = Math.round((start.x / plotWidth) * plotResolution);
        const endIndex = Math.round((end.x / plotWidth) * plotResolution);
        // global.log(ctrl1.x)
        const xOvers = new Array(CURVE_RES);
        for (let i = 0; i < CURVE_RES; i++) {
          const t = i / CURVE_RES;
          // global.log(t)
          const p = this.bezier(start, ctrl1, ctrl2, end, t);
          p.targetIndex = Math.round(((p.x - start.x) / (end.x - start.x)) * (endIndex - startIndex));
          xOvers[i] = p;
        }

        let current = 0;
        let next = 1;
        for (let i = startIndex; i < endIndex; i++) {
          if (!xOvers[next]) break;
          const targetIndex = xOvers[next].targetIndex;
          const dx = targetIndex - i;
          const dy = (xOvers[next].y - xOvers[current].y) / (dx || 1);
          plot[i] = xOvers[current].y + dy;
          // global.log(plot[i]);
          if (i >= targetIndex) {
            current++;
            next++;
          }

        }
      }
    })
    if (this.initialised) global.sendPlot(this.props.paramId, this.normalisePlot());
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
    this.generatePlot();
    const { points, plot, lineColor } = this.state;
    const { gridX, gridY } = this.grid();

    const paths = [];
    for (let i = 0; i < plotResolution - 1; i++) {

      const start = {
        x: (i / plotResolution) * plotWidth,
        y: plot[i]
      }

      const end = {
        x: ((i + 1) / plotResolution) * plotWidth,
        y: plot[i + 1]
      }

      // paths.push(`<path 
      // d="M${start.x} ${start.y} 
      // L${end.x} ${end.y}" 
      // stroke="${lineColor}" stroke-width="2"/>
      // `
      // )
      paths.push(`
        <circle cx="${start.x}" cy="${start.y}" r="${0.5}" fill="${lineColor}" />
      `)
    }

    const circles = points.map((point, index) => {
      if (point.rightNeighbour) {
        const start = point;
        const end = point.rightNeighbour;
        const { ctrl1, ctrl2 } = this.getControlPoints(start, end);
        const curveMod = point.path.controlPoint;

        return `
        <circle cx="${point.x}" cy="${point.y}" r="${point.radius}" fill="${lineColor}" />
        <circle cx="${curveMod.x}" cy="${curveMod.y}" r="${3}" fill="${"#FFEF08"}" />
        <circle cx="${ctrl1.x}" cy="${ctrl1.y}" r="${2}" fill="${"#D519DA"}" />
        <circle cx="${ctrl2.x}" cy="${ctrl2.y}" r="${1.5}" fill="${"#FFFFFF"}" />
        `
      }
      else return `< circle cx = "${point.x}" cy = "${point.y}" r = "${point.radius}" fill = "${lineColor}" />
          `
    })

    const img =
      `
          < svg width = "${boxWidth}" height = "${boxWidth}" viewBox = "0 0 0 0" xmlns = "http://www.w3.org/2000/svg" >
            <rect x="${0}" y="${0}" stroke="green" stroke-width="1" fill="#2a302a" width="${boxWidth}" height="${plotHeight}" />
        ${gridX}
        ${gridY}
        ${paths}
        ${circles}
      </svg >
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
