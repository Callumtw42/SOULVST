import Slider from "./slider"
import { clamp } from "../functions"

export default class Dial extends Slider {
  constructor(props) {
    super(props)
    this.stepNo = 0;
  }

  snap(value, maxVal, stepSize, minStep, maxStep) {
    const vRel = value / maxVal
    const range = maxStep - minStep
    const snapped = Math.round(vRel * range) * stepSize;
    return snapped
  }

  _onMouseDrag(mouseX, mouseY, mouseDownX, mouseDownY) {
    const { max, min, step, height, width, skew } = this.props;
    const startVal = this.valueAtDragStart;
    const snapped = this.snap(mouseDownY - mouseY, width, step, min, max);
    this.stepNo = snapped + startVal;
    const skewed = skew ? Math.pow(skew, snapped + startVal) : snapped + startVal;
    const clamped = clamp(skewed, Math.min(max, min), Math.max(max, min));
    this.props.callBack(clamped);
  }

  _renderVectorGraphics(value, width, height) {
    const { max, min, step, label } = this.props;
    const strokeWidth = 2.0;
    const radius = (Math.min(width - 5, height - 5) * 0.5) - (strokeWidth / 2);

    const arcCircumference = 1.5 * Math.PI * radius;
    const dashArray = [clamp(this.stepNo, 0, Math.max(max, min)) / Math.max(max, min) * arcCircumference, 2.0 * Math.PI * radius];
    return `
        <svg
        width="${width}"
        height="${height}"
        viewBox="0 0 ${width} ${height}"
        version="1.1"
        xmlns="http://www.w3.org/2000/svg">
        <text x="${width / 2}" y="${height}" text-anchor="middle" fill="#D5D5D5"><tspan style="font-size:9.0">${label}</tspan></text>>
        <text x="${width / 2}" y="${(9 / 16) * height}" text-anchor="middle" fill="#D5D5D5"><tspan style="font-size:9.0">${this.props.value}</tspan></text>>
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
}