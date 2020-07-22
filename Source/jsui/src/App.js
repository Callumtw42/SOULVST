import React, { Component } from 'react';
import Dial from './dial';
import { LFO, lfoHeight, lfoWidth } from "./lfo/lfo";
import {
  View,
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
    if (this.state.lfoProps.activeLFO == paramId)
      this.state.lfoProps.activeLFO = "";
    else {
      this.state.lfoProps = {
        x: x - lfoWidth - dialWidth / 2,
        y: y - lfoHeight - dialHeight / 2,
        activeLFO: paramId
      }
    }
    this.setState(this.state);
  }


  render() {
    return (
      <View {...styles.container}>
        <View {...styles.dials}>
          <Dial {...this.sliderCallBacks} paramId="volume"  {...styles.dial} />
          <Dial {...this.sliderCallBacks} paramId="ampAttack"  {...styles.dial} />
          <Dial {...this.sliderCallBacks} paramId="ampDecay" {...styles.dial} />
          <Dial {...this.sliderCallBacks} paramId="ampSustain" {...styles.dial} />
          <Dial {...this.sliderCallBacks} paramId="ampRelease" {...styles.dial} />
          <Dial {...this.sliderCallBacks} paramId="detune" {...styles.dial} />
        </View >
        <View {...styles.dials}>
          <Dial {...this.sliderCallBacks} paramId="voiceCount" {...styles.dial} />
          <Dial {...this.sliderCallBacks} paramId="cutoff" {...styles.dial} />
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
  }
};

export default App;
