import ParameterValueStore from './ParameterValueStore';
import React, { Component } from 'react';
// import Point from "./point"
import {
  Image,
  Text,
  View,
} from 'juce-blueprint';

class LFO extends Component {
  constructor(props) {
    super(props);
    this._onMouseDown = this._onMouseDown.bind(this);
    this._onMouseDrag = this._onMouseDrag.bind(this);
    this._onMouseUp = this._onMouseUp.bind(this);
    // this.log = this.log.bind(this);
    this.state = {
      points: [
        {
          x: 50,
          y: 50,
          radius: 10,
          isSelected: false
        },

        {
          x: 25,
          y: 25,
          radius: 10,
          isSelected: false
        }
      ]
    }
  }

  componentDidMount() {
    // this.state.points.push(
    //   {
    //     x: 50,
    //     y: 50,
    //     radius: 10,
    //     isSelected: false
    //   },

    //   {
    //     x: 25,
    //     y: 25,
    //     radius: 10,
    //     isSelected: false
    //   }
    // )
  }

  componentWillUnmount() {
  }



  log(s) {
    global.log(s);
  }

  _onMouseDrag(mouseX, mouseY, mouseDownX, mouseDownY) {

    let dx = (mouseX - mouseDownX) / 25; //
    let dy = (mouseY - mouseDownY) / 25;

    this.state.points.forEach((point) => {
      if (point.isSelected) {
        point.x += dx;
        point.y += dy;
      }

    })

    this.setState(this.state);
  }

  _onMouseDown(mouseX, mouseY) {

    let mouseIsOverlapping = (point) => {
      if (
        mouseX > point.x - point.radius
        && mouseX < point.x + point.radius
        && mouseY > point.y - point.radius
        && mouseY < point.y + point.radius
      )
        return true
      else return false;
    }

    this.state.points.forEach((point) => {
      if (mouseIsOverlapping(point)) {
        point.isSelected = true;
        log("SELECTED");
      }
    })
  }

  _onMouseUp(mouseX, mouseY) {
    this.state.points.forEach((point) => {
      if (point.isSelected) {
        point.isSelected = false;
      }
    })
  }

  _svg() {

    const circles = this.state.points.map((point) => {
      return `<circle cx="${point.x}" cy="${point.y}" r="${point.radius}" stroke="green" stroke-width="1" fill="yellow" />`
    })

    // [

    //   `<circle cx="${50}" cy="${50}" r="10" stroke="green" stroke-width="1" fill="yellow" />`,
    //   `<circle cx="${25}" cy="${25}" r="10" stroke="green" stroke-width="1" fill="yellow" />`
    // ]
    const img =
      `
      <svg width="0" height="0" viewBox="0 0 0 0" xmlns="http://www.w3.org/2000/svg">
        <rect x="${0}" y="${0}" stroke="green" stroke-width="1" fill="red" width="100" height="100" />
      ${circles}
      </svg>

    `
    return img;
  }

  render() {
    // log("Hello");
    return (
      <View {...styles.container}
        onMouseDown={this._onMouseDown}
        onMouseUp={this._onMouseUp}
        onMouseDrag={this._onMouseDrag}
      >
        <Image {...styles.canvas} source={this._svg()} />
      </View>
    );
  }
}

const styles = {
  container: {
    'flex-direction': 'column',
    'height': 100.0,
    'width': 100.0,
    // 'justify-content': 'center',
    // 'align-items': 'center',
    'position': 'relative',
    'margin': 5.0
    // 'text-overflow':'ellipsis'
  },
  canvas: {
    'flex': 1.0,
    'height': 100.0,
    'width': 100.0,
    'position': 'absolute',
    'left': 0.0,
    'top': 0.0,
    'interceptClickEvents': false,
    // 'transform-rotate': Math.PI * 1.25,
  },
  text: {
    'position': 'relative',
    'color': 'ff626262',
    'font-size': 100.0,
    'line-spacing': 1.6,
    'align-self': 'center'
  },
  label: {
    'flex': 1.0,
    'justify-content': 'center',
    'align-items': 'center',
    'interceptClickEvents': false,
  },
};

export default LFO;
