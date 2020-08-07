import React, { Component } from 'react'
import {
    Image,
    View,
} from 'juce-blueprint';

export default class Button extends Component {

    constructor(props) {
        super(props)

        this.state = {
            on: this.props.on
        }
    }

    _onMouseDown() {
        this.setState({
            on: !this.state.on
        })
        this.props.callBack(this.state.on);
    }

    getColor() {
        if (this.state.on)
            return this.props.color
        else return "#626262"
    }


    svg() {
        const { height, width } = this.props;
        return `
        <svg width="${width}" height="${height}" viewBox="0 0 0 0" xmlns="http://www.w3.org/2000/svg">
            <circle cx="${width / 2}" cy="${height / 2}" r="${width / 3}" fill="${this.getColor()}" />
        </svg>
        `
    }

    render() {
        return (
            <View {...styles.container}{...this.props} onMouseDown={this._onMouseDown.bind(this)}>
                <Image {...styles.canvas} source={this.svg()} />
            </View>
        )
    }
}

const styles = {
    container: {
        'flex-direction': 'column',
        'background-color': '#00333333',
    },

    canvas: {
        'height': '100%',
        'width': '100%',
        'interceptClickEvents': false,

        'position': 'absolute'
    },
}

