import Label from './Label';
import Meter from './Meter';
import React, { Component } from 'react';
import Slider from './Slider';
import LFO from "./lfo";
import {
  Canvas,
  Image,
  View,
  Text,
} from 'juce-blueprint';

class App extends Component {
  render() {
    return (
      <View {...styles.container}>
        <LFO></LFO>
        <Slider paramId="volume" id="volume" x={30} y={30} {...styles.knob} />
        <Slider paramId="ampAttack" x={35} y={35} {...styles.knob} />
        <Slider paramId="ampDecay" x={40} y={40}{...styles.knob} />
        <Slider paramId="ampSustain" x={45} y={45}{...styles.knob} />
        <Slider paramId="ampRelease" x={50} y={50}{...styles.knob} />
        <Slider paramId="detune" x={55} y={55}{...styles.knob} />
        {/* <Slider paramId="voiceCount" {...styles.knob} />
          <Slider paramId="cutoff" {...styles.knob} /> */}
        {/* <Slider paramId="lfoIn" {...styles.knob} /> */}
      </View >
    );
  }
}

const styles = {
  container: {
    // 'flex-direction': 'column',
    'width': '100%',
    'height': '100%',
    'background-color': 'ff17191f',
    // 'justify-content': 'center',
  },
  content: {
    'border-style': 'solid',
    'border-color': 'red',
    'justify-content': 'center',
    'padding': 25.0,
    // 'position': 'sticky',
    // 'max-width': 600,
    // 'height': 100, 
    'background-color': 'ff171910',
    // 'position': 'relative',
  },
  knob: {
    'width': 50.0,
    'height': 50.0,
    // 'margin': 5.0
  },
};

export default App;
