import Slider from "./slider"

export default class Dial extends Slider {
  constructor(props) {
    super(props)
  }

  _onMouseDrag(mouseX, mouseY, mouseDownX, mouseDownY) {
    const { max, min, step, height, width } = this.props;
    const newVal = this.snap(height - mouseY, height, step, min, max)
    this.props.callBack(newVal);
  }

  _renderVectorGraphics(value, width, height) {
    const { max, min, step, label } = this.props;
    const strokeWidth = 2.0;
    const radius = (Math.min(width - 5, height - 5) * 0.5) - (strokeWidth / 2);

    const arcCircumference = 1.5 * Math.PI * radius;
    const dashArray = [(value - min) / (max - min) * arcCircumference, 2.0 * Math.PI * radius];
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