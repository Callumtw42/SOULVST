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
        const paramState = ParameterValueStore.getParameterState(this.props.paramId);
        this.state = {
            width: this.props.width,
            height: this.props.height,
            value: 0.0
        };
    }

    _onMouseDown(mouseX, mouseY) {
        this.setState({ value: clamp(mouseX / this.state.width, 0, 1) });
        global.sendLFOSpeed(this.props.paramId, this.state.value);
    }

    _onMouseDrag(mouseX, mouseY, mouseDownX, mouseDownY) {
        this.setState({ value: clamp(mouseX / this.state.width, 0, 1) });
        global.sendLFOSpeed(this.props.paramId, this.state.value);
    }

    _renderVectorGraphics(value, width, height) {
        const cx = width * 0.5;
        const cy = height * 0.5;
        const strokeWidth = 2.0;
        const radius = (Math.min(width, height) * 0.5) - (strokeWidth / 2);
        const arcCircumference = 1.5 * Math.PI * radius;
        const dashArray = [value * arcCircumference, 2.0 * Math.PI * radius];
        return `
      <svg 
        width="${width}"
        height="${height}"
        viewBox="0 0 ${width} ${height}"
        version="1.1"
        xmlns="http://www.w3.org/2000/svg">

        <path d="M0 ${height / 2} L${width} ${height / 2} Z" stroke="${COLOR}" />
        <circle
          cx="${value * width}"
          cy="${height / 2}"
          r="${radius}"
          stroke="#626262"
          stroke-width="${strokeWidth}"
          stroke-dasharray="${[arcCircumference, arcCircumference].join(',')}"
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
            >
                <Image {...styles.canvas} source={this._renderVectorGraphics(value, width, height)} />
            </View>
        </View>
        );
    }
}

const styles = {
    container: {
        'flex-direction': 'column',
        'background-color': '0f62ffff',
    },
    canvas: {
        'height': '100%',
        'width': '100%',
        'interceptClickEvents': false,

        'position': 'absolute'
    },
};

export default Slider;
