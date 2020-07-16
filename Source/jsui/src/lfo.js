import ParameterValueStore from './ParameterValueStore';
import React, { Component } from 'react';
// import Point from "./point"
import {
  Image,
  Text,
  View,
} from 'juce-blueprint';

const pointRadius = 5;
const canvasHeight = 100;
const canvasWidth = 100;

class Mouse {
  constructor(mouseX, mouseY) {
    this.x = mouseX;
    this.y = mouseY;
  };
  isOverlapping(point) {
    if (
      this.x > point.x - point.radius
      && this.x < point.x + point.radius
      && this.y > point.y - point.radius
      && this.y < point.y + point.radius
    )
      return true
    else return false;
  }
}

class LFO extends Component {
  constructor(props) {
    super(props);
    this._onMouseDown = this._onMouseDown.bind(this);
    this._onMouseDrag = this._onMouseDrag.bind(this);
    this._onMouseUp = this._onMouseUp.bind(this);
    this._onMouseDoubleClick = this._onMouseDoubleClick.bind(this);
    // this.log = this.log.bind(this);
    this.state = {
      points: []
    }
  }

  componentDidMount() {
    this.state.points.push(
      {
        x: 0,
        y: canvasHeight / 2,
        radius: pointRadius,
        isSelected: false,
        isBound: true
      },
      {
        x: canvasWidth,
        y: canvasHeight / 2,
        radius: pointRadius,
        isSelected: false,
        isBound: true
      },
      {
        x: 75,
        y: 75,
        radius: pointRadius,
        isSelected: false
      }
    )
    this.setState(this.state);
  }

  componentWillUnmount() {
  }

  log(s) {
    global.log(s);
  }

  _onMouseDoubleClick(mouseX, mouseY) {

    let mouse = new Mouse(mouseX, mouseY);
    let removing = false;

    for (const point of this.state.points) {
      if (mouse.isOverlapping(point) && !point.isBound) {
        const index = this.state.points.indexOf(point);
        this.state.points.splice(index, 1);
        removing = true;
        break;
      }
    }

    if (!removing) {
      this.state.points.push( //NEXT: Change this to insert a point depending on it's neighbours x value
        {
          x: mouseX,
          y: mouseY,
          radius: pointRadius,
          isSelected: false
        }
      )
    }

    this.setState(this.state);
  }

  clamp(val, min, max) {
    return val > max ? max : val < min ? min : val;
  }

  _onMouseDrag(mouseX, mouseY, mouseDownX, mouseDownY) {

    this.state.points.forEach((point) => {
      if (point.isSelected) {
        point.y = this.clamp(mouseY, 0, canvasHeight);
        if (!point.isBound) {
          point.x = this.clamp(mouseX, 0, canvasWidth);
        }
      }

    })

    this.setState(this.state);
  }

  _onMouseDown(mouseX, mouseY) {
    let mouse = new Mouse(mouseX, mouseY);
    this.state.points.forEach((point) => {
      if (mouse.isOverlapping(point)) {
        point.isSelected = true;
      }
    });
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
        <rect x="${0}" y="${0}" stroke="green" stroke-width="1" fill="red" width="${canvasHeight}" height="${canvasWidth}" />
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
        onMouseDoubleClick={this._onMouseDoubleClick}
      >
        <Image {...styles.canvas} source={this._svg()} />
      </View>
    );
  }
}

const styles = {
  container: {
    'flex-direction': 'column',
    'height': canvasHeight,
    'width': canvasWidth,
    // 'justify-content': 'center',
    // 'align-items': 'center',
    'position': 'relative',
    'margin': 5.0
    // 'text-overflow':'ellipsis'
  },
  canvas: {
    'flex': 1.0,
    'height': canvasHeight,
    'width': canvasWidth,
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
