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
        <View {...styles.content}>
          <Slider paramId="volume" {...styles.knob} />
          <Slider paramId="ampAttack" {...styles.knob} />
          <Slider paramId="ampDecay" {...styles.knob} />
          <Slider paramId="ampSustain" {...styles.knob} />
          <Slider paramId="ampRelease" {...styles.knob} />
          <Slider paramId="detune" {...styles.knob} />
        </View>
        <View {...styles.content}>
          <Slider paramId="voiceCount" {...styles.knob} />
          <Slider paramId="cutoff" {...styles.knob} />
          <Slider paramId="lfoIn" {...styles.knob} />
        </View>
        <View {...styles.content}>
          <LFO {...styles.knob} />
        </View>
      </View>
    );
  }
}

const styles = {
  container: {
    'flex-direction': 'column',
    'width': '100%',
    'height': '100%',
    'background-color': 'ff17191f',
  },
  content: {
    'padding': 5.0,
    'max-width': 600,
    'position': 'relative'
  },
  knob: {
    'width': 50.0,
    'height': 50.0,
    // 'margin': 5.0
  },
};

export default App;
