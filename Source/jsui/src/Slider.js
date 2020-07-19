import ParameterValueStore from './ParameterValueStore';
import React, { Component } from 'react';
import Label from './Label';
import {
  Image,
  Text,
  View,
} from 'juce-blueprint';

import LFO from "./lfo";


class Slider extends Component {
  constructor(props) {
    super(props);

    this._onMeasure = this._onMeasure.bind(this);
    this._onMouseDown = this._onMouseDown.bind(this);
    this._onMouseUp = this._onMouseUp.bind(this);
    this._onMouseDrag = this._onMouseDrag.bind(this);
    this._renderVectorGraphics = this._renderVectorGraphics.bind(this);
    this._onParameterValueChange = this._onParameterValueChange.bind(this);
    this._onMouseDoubleClick = this._onMouseDoubleClick.bind(this);

    // During a drag, we hold the value at which the drag started here to
    // ensure smooth behavior while the component state is being updated.
    this._valueAtDragStart = 0.0;

    const paramState = ParameterValueStore.getParameterState(this.props.paramId);
    const initialValue = typeof paramState.currentValue === 'number' ?
      paramState.currentValue : 0.0;

    this.state = {
      width: 0,
      height: 0,
      value: initialValue,
    };
  }

  componentDidMount() {
    ParameterValueStore.addListener(
      ParameterValueStore.CHANGE_EVENT,
      this._onParameterValueChange
    );
  }

  componentWillUnmount() {
    ParameterValueStore.removeListener(
      ParameterValueStore.CHANGE_EVENT,
      this._onParameterValueChange
    );
  }

  _onMouseDoubleClick(mouseX, mouseY) {
    const x = global.getMouseX();
    const y = global.getMouseY();
    this.props.spawnLFO(this.props.paramId, x, y)
  }

  _onMeasure(width, height) {
    this.setState({
      width: width,
      height: height,
    });
  }

  _onMouseDown(mouseX, mouseY) {
    this._valueAtDragStart = this.state.value;
    global.beginParameterChangeGesture(this.props.paramId);
  }

  _onMouseUp(mouseX, mouseY) {
    global.endParameterChangeGesture(this.props.paramId);
  }

  _onMouseDrag(mouseX, mouseY, mouseDownX, mouseDownY) {
    // Component vectors
    // let dx = mouseX - mouseDownX;
    let dy = mouseDownY - mouseY;

    // Delta
    // let dm = dx + dy;
    let sensitivity = (1.0 / 200.0);
    let value = Math.max(0.0, Math.min(1.0, this._valueAtDragStart + dy * sensitivity));


    if (typeof this.props.paramId === 'string' && this.props.paramId.length > 0) {
      global.setParameterValueNotifyingHost(this.props.paramId, value);
    }
  }

  _onParameterValueChange(paramId) {
    const shouldUpdate = typeof this.props.paramId === 'string' &&
      this.props.paramId.length > 0 &&
      this.props.paramId === paramId;

    if (shouldUpdate) {
      const state = ParameterValueStore.getParameterState(paramId);

      this.setState({
        defaultValue: state.defaultValue,
        value: state.currentValue,
      });
    }
  }

  _renderVectorGraphics(value, width, height) {
    const cx = width * 0.5;
    const cy = height * 0.5;
    const strokeWidth = 2.0;

    // Note that we nudge the radius by half the stroke width; this is because
    // the stroke will extend outwards in both directions from the given coordinates,
    // which gets clipped if we try to draw the circle perfectly on the edge of the
    // image. We nudge it in so that no part of the path gets clipped.
    const radius = (Math.min(width, height) * 0.5) - (strokeWidth / 2);

    // Animate the arc by stroke-dasharray, where the length of the dash is
    // related to the value property and the length of the space takes up the
    // rest of the circle.
    const arcCircumference = 1.5 * Math.PI * radius;
    const dashArray = [value * arcCircumference, 2.0 * Math.PI * radius];

    return `
      <svg
        width="${width}"
        height="${height}"
        viewBox="0 0 ${width} ${height}"
        version="1.1"
        xmlns="http://www.w3.org/2000/svg">
        <circle
          cx="${cx}"
          cy="${cy}"
          r="${radius}"
          stroke="#626262"
          stroke-width="${strokeWidth}"
          stroke-dasharray="${[arcCircumference, arcCircumference].join(',')}"
          fill="none" />
        <circle
          cx="${cx}"
          cy="${cy}"
          r="${radius}"
          stroke="#66FDCF"
          stroke-width="${strokeWidth}"
          stroke-dasharray="${dashArray.join(',')}"
          fill="none" />
      </svg>
    `;
  }

  render() {
    const { value, width, height } = this.state;
    // this.props.addLFO(this.props.paramId);

    return (<View {...styles.container}>

      <View
        {...this.props}
        onMeasure={this._onMeasure}
        onMouseDown={this._onMouseDown}
        onMouseUp={this._onMouseUp}
        onMouseDrag={this._onMouseDrag}
        onMouseDoubleClick={this._onMouseDoubleClick}>
        <Image {...styles.canvas} source={this._renderVectorGraphics(value, width, height)} />
        <Label paramId={this.props.paramId} {...styles.label} />
      </View>
      <Text {...styles.nameText}>{this.props.paramId}</Text>
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
    'top': 10.0,
    'interceptClickEvents': false,
    'transform-rotate': Math.PI * 1.25,
    'position': 'absolute'
  },
  nameText: {
    'color': 'ff626262',
    'font-size': 12.0,
    'line-spacing': 1.6,
    'left': 7
  },
  label: {
    'flex': 1.0,
    'justify-content': 'center',
    'align-items': 'center',
    'interceptClickEvents': false,
    'top': 10
  },
};



export default Slider;
