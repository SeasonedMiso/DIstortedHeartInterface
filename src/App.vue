<script>
import { reactive, toRefs } from 'vue'
import VueSlider from 'vue-slider-component'
import 'vue-slider-component/theme/antd.css'
export default {
  name: "App",
  components: {
    VueSlider,
  },
  setup() {
    const sliderData = reactive({ value: 0 })
    return toRefs(sliderData)
  },
  methods: {
    // test() {
    //   window.ipcRenderer.send("arduinoTest", "arg here") // or any other ipcRenderer method you want to invoke
    //   console.log(window.ipcRenderer)
    // }
    decrementActivePreset() {
      this.activePreset = (this.activePreset > 0) ? this.activePreset - 1 : 2;
    },
    incrementActivePreset() {
      this.activePreset = (this.activePreset > 1) ? 0 : this.activePreset + 1;
    }
  },
  data() {
    return {
      activePreset: 0,
      presets: [
        {
          lpf: 0,
          hpf: 0,
          gateThreshold: 0,
          compThreshold: 0,
          odGain: 0,
          volume: 0,
        },
        {
          lpf: 0,
          hpf: 0,
          gateThreshold: 0,
          compThreshold: 0,
          odGain: 0,
          volume: 0,
        },
        {
          lpf: 0,
          hpf: 0,
          gateThreshold: 0,
          compThreshold: 0,
          odGain: 0,
          volume: 0,
        }
      ]
    }
  }
};
</script>
<script setup>
</script>

<template >
  <!-- <link href="../dist/output.css" rel="stylesheet"> -->
  <div class="container" style="position: relative; max-height:100vh;">
    <!-- <div class="flex-container"> -->
      <!-- <button @click="decrementActivePreset()" style="   width: 10%;">◀︎</button> -->
      <h1>Preset {{ activePreset + 1 }}</h1>
      <!-- <button @click="incrementActivePreset()" style=" width: 10%;">▶︎</button> -->
    <!-- </div> -->
    <button @click="decrementActivePreset()" style=" margin: 0 auto; margin-bottom: 30px; width: 90%;">▲</button>
    <div style=" margin: 0 auto; width: 80%;">
      LowPassCutoff:{{ presets[activePreset].lpf * 50 + "hz" }}
      <vue-slider ref=" slider" v-model="presets[activePreset].lpf" />
      HighPassCutoff:{{ 20000 - presets[activePreset].hpf * 100 + "hz" }}
      <vue-slider ref="slider" v-model="presets[activePreset].hpf" />
      gateThreshold:{{ presets[activePreset].gateThreshold }}
      <vue-slider ref="slider" v-model="presets[activePreset].gateThreshold" />
      compThreshold:{{ presets[activePreset].compThreshold }}
      <vue-slider ref="slider" v-model="presets[activePreset].compThreshold" />
      odGain:{{ presets[activePreset].odGain }}
      <vue-slider ref="slider" v-model="presets[activePreset].odGain" />
      volume:{{ presets[activePreset].volume }}
      <vue-slider ref="slider" v-model="presets[activePreset].volume" />

    </div>
    <button @click="incrementActivePreset()" style="margin: 0 auto; margin-bottom: 30px; width: 90%;">▼</button>
    <button @click="test" class="text-3xl font-bold underline" style=" width: 80%; margin: 0 auto; margin-top:">Save
      Preset</button>

  </div>
</template>

<style scoped>
.logo.vite:hover {
  filter: drop-shadow(0 0 2em #747bff);
}

.logo.vue:hover {
  filter: drop-shadow(0 0 2em #249b73);
}

.vue-slider {
  margin-bottom: 30px;
}
</style>
