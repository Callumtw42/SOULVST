import Slider from "./slider"
import { clamp } from "../functions"
import React, { Component } from "react";
import {
  Image,
  Text,
  View,
} from 'juce-blueprint';
const PLOT_RES = 128
const MAX_MOUSE_DISP = 200
export default class Dial extends Component {
  constructor(props) {
    super(props)
    this._onMouseDrag = this._onMouseDrag.bind(this);
    this._onMouseDown = this._onMouseDown.bind(this);
    this.valueAtDragStart = this.props.value
    this.plot = new Array(128)
  }

  componentDidMount() {
    for (let index = 0; index < PLOT_RES; index++) {
      this.plot[index] = this.populate(index);
    }
  }

  populate(i) {
    const { end, start, step, height, width, skew, label } = this.props;
    const snapped = this.snap(i, PLOT_RES, step, start, end);
    // if (snapped != this.plot[i - 1])
    //   this.stepSize *= skew;
    // const skewed = skew ? Math.pow(skew, snapped) : snapped;
    global.log(new String(label + ": " + snapped).toString());
    return snapped;
  }

  snap(value, maxVal, stepSize, startStep, endStep) {
    const vRel = value / maxVal
    const stepRange = endStep - startStep
    const numSteps = (stepRange / stepSize)
    const stepNumber = Math.round(vRel * numSteps)
    const skewed = this.props.skew ? Math.pow(this.props.skew, stepNumber) : stepNumber;
    const snapped = skewed * (stepSize)
    return snapped
  }

  _onMouseDown(mouseX, mouseY) {
    const { end, start, step, height, width, value } = this.props;
    this.valueAtDragStart = value;
  }

  rememberStart() {
    const { start, end } = this.props;
    const stepRange = end - start
    return this.valueAtDragStart / stepRange
  }

  getRelativeValue() {
    const { start, end, value } = this.props;
    const stepRange = end - start;
    return value / stepRange;
  }

  _onMouseDrag(mouseX, mouseY, mouseDownX, mouseDownY) {
    const { end, start, step, height, width, skew } = this.props;
    const relStart = this.rememberStart();

    const dy = mouseDownY - mouseY;
    const rdy = clamp((dy / MAX_MOUSE_DISP) + relStart, 0, 1);
    const index = Math.round((rdy) * (PLOT_RES - 1))
    this.props.callBack(this.plot[index]);
  }

  _renderVectorGraphics(value, width, height) {
    const { end, start, step, label } = this.props;
    const strokeWidth = 2.0;
    const radius = (Math.min(width - 5, height - 5) * 0.5) - (strokeWidth / 2);
    const arcCircumference = 1.5 * Math.PI * radius;
    const dashArray = [value * arcCircumference, 2.0 * Math.PI * radius];
    return `
        <svg
        width="${width}"
        height="${height}"
        viewBox="0 0 ${width} ${height}"
        version="1.1"
        xmlns="http://www.w3.org/2000/svg">
        <text x="${width / 2}" y="${height}" text-anchor="middle" fill="#D5D5D5"><tspan style="font-size:9.0">${label}</tspan></text>>
        <text x="${width / 2}" y="${(9 / 16) * height}" text-anchor="middle" fill="#D5D5D5"><tspan style="font-size:9.0">${this.props.value.toFixed(2)}</tspan></text>>
        <circle
        transform="rotate(-135,${width / 2},${height / 2})"
          cx="${width / 2}"
          cy="${height / 2}"
          r="${radius}"
          stroke="#626262"
          stroke-width="${strokeWidth}"
          stroke-dasharray="${[arcCircumference, arcCircumference].join(',')}"
          fill="none" />
        <circle
        transform="rotate(-135,${width / 2},${height / 2})"
          cx="${width / 2}"
          cy="${height / 2}"
          r="${radius}"
          stroke="#66FDCF"
          stroke-width="${strokeWidth}"
          stroke-dasharray="${dashArray.join(',')}"
          fill="none" />
      </svg>>
    `;

  }

  render() {
    const { value, width, height } = this.props
    return (<View {...styles.container}>
      <View
        height={height}
        width={width}
        onMouseDown={this._onMouseDown}
        onMouseDrag={this._onMouseDrag}
      >
        <Image {...styles.canvas} source={this._renderVectorGraphics(this.getRelativeValue(), width, height)} />
      </View>
    </View>
    );
  }
}


Dial.defaultProps = { skew: 0 }

const styles = {
  container: {
    'flex-direction': 'column',
    'background-color': '#00333333',
  },
  text: {
    'color': 'ff626262',
    'font-size': 8.0,
  },
  canvas: {
    'height': '100%',
    'width': '100%',
    'interceptClickEvents': false,

    'position': 'absolute'
  },
};