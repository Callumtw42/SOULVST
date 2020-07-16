import ParameterValueStore from './ParameterValueStore';
import React, { Component } from 'react';
import {
  Image,
  Text,
  View,
} from 'juce-blueprint';

const pointRadius = 4;
const canvasHeight = 100;
const canvasWidth = 100;
const plotResolution = 128;

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

class NodeList {
  constructor(head, tail) {
    this.list = [head, tail];
    head.rightNeighbour = tail;
    head.leftNeighbour = null;
    tail.rightNeighbour = null;
    tail.leftNeighbour = head;
  }

  insertAfter(index, node) {
    this.list.splice(
      index + 1,
      0,
      node
    )
    node.leftNeighbour = this.list[index];
    node.leftNeighbour.rightNeighbour = node;

    node.rightNeighbour = (this.list[index + 2]) ? this.list[index + 2] : null;
    node.rightNeighbour.leftNeighbour = node;
  }

  remove(index) {
    const point = this.list[index];
    const leftNeighbour = point.leftNeighbour;
    const rightNeighbour = point.rightNeighbour;

    this.list.splice(index, 1);
    this.list[index].leftNeighbour = leftNeighbour;
    this.list[index - 1].rightNeighbour = rightNeighbour;
  }

  forEach(...args) {
    return this.list.forEach(...args);
  }

  indexOf(...args) {
    return this.list.indexOf(...args);
  }

  splice(...args) {
    return this.list.splice(...args);
  }

  map(...args) {
    return this.list.map(...args);
  }

  get() {
    return this.list;
  }
}

class LFO extends Component {
  constructor(props) {
    super(props);
    this._onMouseDown = this._onMouseDown.bind(this);
    this._onMouseDrag = this._onMouseDrag.bind(this);
    this._onMouseUp = this._onMouseUp.bind(this);
    this._onMouseDoubleClick = this._onMouseDoubleClick.bind(this);
    this.initialised = false;

    this.nodeList = new NodeList(
      {
        x: 0,
        y: canvasHeight,
        radius: pointRadius,
        isSelected: false,
        isBound: true,
        leftNeighbour: null,
        rightNeighbour: null
      },
      {
        x: canvasWidth,
        y: canvasHeight,
        radius: pointRadius,
        isSelected: false,
        isBound: true,
        leftNeighbour: null,
        rightNeighbour: null
      }
    )

    this.state = {
      points: this.nodeList,
      plot: new Array(plotResolution)
    }
  }

  componentDidMount() {

    this.nodeList.insertAfter(0,
      {
        x: canvasWidth / 2,
        y: 0,
        radius: pointRadius,
        isSelected: false,
        isBound: false,
        leftNeighbour: null,
        rightNeighbour: null
      },
    )
    this.setState(this.state);
  }

  componentWillUnmount() {
  }

  log(s) {
    global.log();
  }

  _onMouseDoubleClick(mouseX, mouseY) {
    const { points } = this.state;

    let mouse = new Mouse(mouseX, mouseY);
    let removing = false;

    for (const point of points.get()) {
      if (mouse.isOverlapping(point) && !point.isBound) {
        const index = points.indexOf(point);
        points.remove(index);
        removing = true;
        break;
      }
    }

    if (!removing) {
      let leftNeighbour = points.get()[0];
      let shortestLeftPath = mouse.x - leftNeighbour.x;

      for (const point of points.get()) {
        let leftPath = mouse.x - point.x;
        if (leftPath < 0)
          break;
        else if (leftPath <= shortestLeftPath) {
          leftNeighbour = point;
        }
      }
      const index = points.indexOf(leftNeighbour);
      // log(index);

      points.insertAfter(
        index,
        {
          x: mouseX,
          y: mouseY,
          radius: pointRadius,
          isSelected: false,
        }//
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
          point.x = this.clamp(mouseX, point.leftNeighbour.x, point.rightNeighbour.x);
        }
      }

    })

    this.setState(this.state);
  }

  _onMouseDown(mouseX, mouseY) {
    this.initialised = true;
    
    const points = this.state.points;
    let mouse = new Mouse(mouseX, mouseY);
    points.forEach((point) => {
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

  generatePlot() {
    const { points, plot } = this.state;

    let startIndex = 0;
    points.forEach((point, index) => {

      if (point.rightNeighbour != null) {
        const rightNeighbourY = point.rightNeighbour.y / canvasHeight;
        const rightNeighbourX = point.rightNeighbour.x / canvasWidth;
        const x = point.x / canvasWidth;
        const y = point.y / canvasHeight;

        const targetIndex = Math.round(rightNeighbourX * plotResolution);
        const dy = (rightNeighbourY - y) / (rightNeighbourX - x) / plotResolution;
        let value = y;
        for (let i = startIndex; i < targetIndex; i++) {
          plot[i] = 1 - value;
          value += dy;
        }
        startIndex = targetIndex;
      }
    })

    if (this.initialised) global.sendPlot("lfoIn", plot.toString());//
    // log("#############################################################################");
    // plot.forEach((p, i) => {
    //   log(plot[i]); //
    // })
    // log("#############################################################################");
  }

  _svg() {
    const { points, plot } = this.state;

    this.generatePlot();

    const paths = points.map((point) => {
      if (point.rightNeighbour) return `<path d="M${point.x} ${point.y} L${point.rightNeighbour.x} ${point.rightNeighbour.y} Z" stroke="#3ade3a" stroke-width="2"/>`
      else return ``;
    })

    const circles = points.map((point, index) => {
      return `
      <circle cx="${point.x}" cy="${point.y}" r="${point.radius}" fill="#3ade3a" />
      ${paths[index]}
        `
    })
    const img =
      `
      <svg width="${canvasWidth}" height="${canvasHeight}" viewBox="0 0 0 0" xmlns="http://www.w3.org/2000/svg">
        <rect x="${0}" y="${0}" stroke="green" stroke-width="1" fill="#2a302a" width="${canvasHeight}" height="${canvasWidth}" />
        ${circles}
      </svg>
      `
    return img;
  }

  render() {
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