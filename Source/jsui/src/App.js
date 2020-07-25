import React, { Component } from 'react';
import ParamDial from './paramdial';
import { LFO, lfoHeight, lfoWidth } from "./lfo/lfo";
import {
  View,
  Text
} from 'juce-blueprint';

const dialHeight = 50;
const dialWidth = 50;

function SliderLabel(props) {
  const styles = {
    text: {
      "top": props.y,
      "left": props.x,
      "color": "ff62ffff",
      "position": "absolute"
    }
  }
  return <Text {...styles.text}>{props.text}</Text>
}

class App extends Component {
  constructor() {
    super()
    this.state = {
      lfoProps: {
        spawnLabel: this.spawnLabel.bind(this),
      },
      labelProps: {
        x: 0,
        y: 0,
        text: ""
      }
    }
    this.sliderCallBacks = {
      spawnLFO: this.spawnLFO.bind(this),
    }
  }

  spawnLabel(x, y, text) {
    this.setState({
      labelProps: {
        x: x,
        y: y,
        text: text
      }
    })
  }

  spawnLFO(paramId, x, y) {
    if (this.state.lfoProps.activeLFO == paramId)
      this.state.lfoProps.activeLFO = "";
    else {
      this.state.lfoProps = {
        x: x - lfoWidth - dialWidth / 2,
        y: y - lfoHeight - dialHeight / 2,
        activeLFO: paramId,
        spawnLabel: this.spawnLabel.bind(this),
      }
    }
    this.setState(this.state);
  }


  render() {
    const { lfoProps, labelProps } = this.state
    return (
      <View {...styles.container}>
        <View {...styles.dials}>
          <ParamDial {...this.sliderCallBacks} paramId="volume"  {...styles.dial} />
          <ParamDial {...this.sliderCallBacks} paramId="ampAttack"  {...styles.dial} />
          <ParamDial {...this.sliderCallBacks} paramId="ampDecay" {...styles.dial} />
          <ParamDial {...this.sliderCallBacks} paramId="ampSustain" {...styles.dial} />
          <ParamDial {...this.sliderCallBacks} paramId="ampRelease" {...styles.dial} />
          <ParamDial {...this.sliderCallBacks} paramId="detune" {...styles.dial} />
        </View >
        <View {...styles.dials}>
          <ParamDial {...this.sliderCallBacks} paramId="voiceCount" {...styles.dial} />
          <ParamDial {...this.sliderCallBacks} paramId="cutoff" {...styles.dial} />
        </View >
        < LFO paramId={"volume"} {...lfoProps} ></LFO >
        < LFO paramId={"ampAttack"} {...lfoProps} ></LFO >
        < LFO paramId={"ampDecay"} {...lfoProps} ></LFO >
        < LFO paramId={"ampSustain"} {...lfoProps} ></LFO >
        < LFO paramId={"ampRelease"} {...lfoProps} ></LFO >
        < LFO paramId={"detune"} {...lfoProps} ></LFO >
        < LFO paramId={"voiceCount"} {...lfoProps} ></LFO >
        < LFO paramId={"cutoff"} {...lfoProps} ></LFO >
        <SliderLabel {...labelProps}></SliderLabel>
      </View >
    );
  }
}

const styles = {

  container: {
    'width': '100%',
    'height': '100%',
    'background-color': 'ff17191f',
    'justify-content': 'center',
    'flex-direction': 'column'
  },
  dials: {
    'border-style': 'solid',
    'border-color': 'red',
    'justify-content': 'center',
    'background-color': 'ff171910',
  },
  dial: {
    'width': dialWidth,
    'height': dialHeight,
  }
};

export default App;
