import Label from './Label';
import Meter from './Meter';
import React, { Component } from 'react';
import Slider from './Slider';
import { LFO, lfoHeight, lfoWidth } from "./lfo";
import {
  Canvas,
  Image,
  View,
  Text,
} from 'juce-blueprint';

const dialHeight = 50;
const dialWidth = 50;

class App extends Component {
  constructor() {
    super()
    this.state = {
      lfoProps: {},
      lfos: [],
    }
    this.sliderCallBacks = {
      spawnLFO: this.spawnLFO.bind(this),
    }
  }

  spawnLFO(paramId, x, y) {
    this.state.lfoProps = {
      x: x - lfoWidth - dialWidth / 2,
      y: y - lfoHeight - dialHeight / 2,
      activeLFO: paramId
    }
    this.setState(this.state);
  }


  render() {
    return (
      <View {...styles.container}>
        <View {...styles.dials}>
          <Slider {...this.sliderCallBacks} paramId="volume"  {...styles.dial} />
          <Slider {...this.sliderCallBacks} paramId="ampAttack"  {...styles.dial} />
          <Slider {...this.sliderCallBacks} paramId="ampDecay" {...styles.dial} />
          <Slider {...this.sliderCallBacks} paramId="ampSustain" {...styles.dial} />
          <Slider {...this.sliderCallBacks} paramId="ampRelease" {...styles.dial} />
          <Slider {...this.sliderCallBacks} paramId="detune" {...styles.dial} />
        </View >
        <View {...styles.dials}>
          <Slider {...this.sliderCallBacks} paramId="voiceCount" {...styles.dial} />
          <Slider {...this.sliderCallBacks} paramId="cutoff" {...styles.dial} />
        </View >
        < LFO paramId={"volume"} {...this.state.lfoProps} ></LFO >
        < LFO paramId={"ampAttack"} {...this.state.lfoProps} ></LFO >
        < LFO paramId={"ampDecay"} {...this.state.lfoProps} ></LFO >
        < LFO paramId={"ampSustain"} {...this.state.lfoProps} ></LFO >
        < LFO paramId={"ampRelease"} {...this.state.lfoProps} ></LFO >
        < LFO paramId={"detune"} {...this.state.lfoProps} ></LFO >
        < LFO paramId={"voiceCount"} {...this.state.lfoProps} ></LFO >
        < LFO paramId={"cutoff"} {...this.state.lfoProps} ></LFO >
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
  },
};

export default App;
