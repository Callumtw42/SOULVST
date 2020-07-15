import ParameterValueStore from './ParameterValueStore';
import React, { Component } from 'react';
import {
    Image,
    Text,
    View,
} from 'juce-blueprint';

class Point extends Component {
    constructor(props) {
        super(props);
        this._onMouseDown = this._onMouseDown.bind(this);
        this.log = this.log.bind(this);
        this.state =
        {
            x: 0,
            y: 0
        }
    }

    componentDidMount() {
    }

    componentWillUnmount() {
    }

    _onmouseDown(){

    }

    log(s) {
        global.log(s);
    }

    render() {
        return <View><Text>HELLO</Text></View>
    }

}

export default Point;