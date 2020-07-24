import { clamp } from "../functions"
import ParameterValueStore from '../ParameterValueStore';
import React, { Component } from 'react';
import Label from '../Label';
import {
    Image,
    Text,
    View,
} from 'juce-blueprint';

const COLOR = "#3bb3eb"

class Slider extends Component {
    constructor(props) {
        super(props);

        this._onMouseDown = this._onMouseDown.bind(this);
        this._onMouseDrag = this._onMouseDrag.bind(this);
        this._onMouseUp = this._onMouseUp.bind(this);
        this.state = {
            width: this.props.width,
            height: this.props.height,
            value: this.props.min
        };
    }

    snap(value, maxVal, stepSize, minStep, maxStep) {
        const { skew } = this.props
        const vRel = value / maxVal
        const range = maxStep - minStep
        const snapped = Math.round(vRel * range) * stepSize + minStep
        global.log(snapped)
        const skewed = skew ? Math.pow(skew, snapped) : snapped
        const clamped = clamp(skewed, Math.min(maxStep, minStep), Math.max(maxStep, minStep));
        return clamped
        // return clamped
    }

    _onMouseUp(mouseX, mouseY) {
        this.props.spawnLabel(0, 0, "")
    }

    _onMouseDown(mouseX, mouseY) {
        const { max, min, step } = this.props;
        const { height, width, value } = this.state
        const newVal = this.snap(mouseX, width, step, min, max)
        this.setState({ value: newVal })
        this.props.callBack(value);
        this.props.spawnLabel(global.getMouseX() - mouseX + width / 2, global.getMouseY() - mouseY + height, value)
    }

    _onMouseDrag(mouseX, mouseY, mouseDownX, mouseDownY) {
        const { max, min, step } = this.props;
        const { height, width, value } = this.state
        const newVal = this.snap(mouseX, width, step, min, max)
        this.setState({ value: newVal })
        this.props.callBack(value);
        this.props.spawnLabel(global.getMouseX() - mouseX + width / 2, global.getMouseY() - mouseY + height, value)
    }

    _renderVectorGraphics(value, width, height) {
        const strokeWidth = 2.0;
        const radius = (Math.min(width, height) * 0.5) - (strokeWidth / 2);
        return `
      <svg 
        width="${width}"
        height="${height}"
        viewBox="0 0 ${width} ${height}"
        version="1.1"
        xmlns="http://www.w3.org/2000/svg">

        <path d="M0 ${height / 2} L${width} ${height / 2} Z" stroke="${COLOR}" />
        <circle
          cx="${(value / (this.props.max)) * width}"
          cy="${height / 2}"
          r="${radius}"
          stroke="#626262"
          stroke-width="${strokeWidth}"
          fill="${COLOR}" />
      </svg>
    `;
    }

    render() {
        const { value, width, height } = this.state;
        return (<View {...styles.container}>
            <View
                {...this.props}
                onMouseDown={this._onMouseDown}
                onMouseDrag={this._onMouseDrag}
                onMouseUp={this._onMouseUp}
            >
                <Image {...styles.canvas} source={this._renderVectorGraphics(value, width, height)} />
                {/* <Text {...styles.text}>{"Grid"}</Text> */}
            </View>
        </View>
        );
    }
}

Slider.defaultProps = { skew: null }

const styles = {
    container: {
        'flex-direction': 'column',
        'background-color': 'ff333333',
    },
    text: {
        'color': 'ff626262',
        'font-size': 8.0,
        // 'line-spacing': 1.6,
        // 'left': 7,
        // 'position': 'absolute'
    },
    canvas: {
        'height': '100%',
        'width': '100%',
        'interceptClickEvents': false,

        'position': 'absolute'
    },
};

export default Slider;
