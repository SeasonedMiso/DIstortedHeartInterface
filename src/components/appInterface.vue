<script>
import { reactive, toRefs } from "vue";
import VueSlider from "vue-slider-component";
import "vue-slider-component/theme/antd.css";
import { invoke } from "@tauri-apps/api/tauri";
// const invoke = window.__TAURI__.invoke

export default {
  name: "appInterface",
  components: {
    VueSlider,
  },
  setup() {
    const sliderData = reactive({ value: 0 });
    invoke("change_preset", {
      presetNo: (1).toString(),
    }).then((message) => {
      //message is timing out before it can be used??
      //why is this broken
      this.presetTemp = message.split(" ");
      this.presets[this.activePreset].lpf = this.presetTemp[1];
      this.presets[this.activePreset].odGain = this.presetTemp[2];
      this.presets[this.activePreset].volume = this.presetTemp[3];
      this.presets[this.activePreset].superDistortion = this.presetTemp[4];
    });
    return toRefs(sliderData);
  },
  methods: {
    decrementActivePreset() {
      this.activePreset = this.activePreset > 0 ? this.activePreset - 1 : 2;
      this.saveBool = true;
      invoke("change_preset", {
        presetNo: (this.activePreset + 1).toString(),
      }).then((message) => {
        this.presetTemp = message.split(" ");
        this.presets[this.activePreset].lpf = this.presetTemp[1];
        this.presets[this.activePreset].odGain = this.presetTemp[2];
        this.presets[this.activePreset].volume = this.presetTemp[3];
        if (this.presetTemp[4] == "t") {
          this.presets[this.activePreset].superDistortion = true;
        } else {
          this.presets[this.activePreset].superDistortion = false;
        }
      });
    },
    incrementActivePreset() {
      this.activePreset = this.activePreset > 1 ? 0 : this.activePreset + 1;
      this.saveBool = true;
      invoke("change_preset", {
        presetNo: (this.activePreset + 1).toString(),
      }).then((message) => {
        this.presetTemp = message.split(" ");
        this.presets[this.activePreset].lpf = this.presetTemp[1];
        this.presets[this.activePreset].odGain = this.presetTemp[2];
        this.presets[this.activePreset].volume = this.presetTemp[3];
        if (this.presetTemp[4] == "t") {
          this.presets[this.activePreset].superDistortion = true;
        } else {
          this.presets[this.activePreset].superDistortion = false;
        }
      });
    },
    presetString(activePreset) {
      let stringOutput = "";
      let currentPreset = this.presets[this.activePreset];
      stringOutput += currentPreset.lpf + " ";
      // stringOutput += currentPreset.hpf + " ";
      // stringOutput += currentPreset.gateThreshold + " ";
      // stringOutput += currentPreset.compThreshold + " ";
      stringOutput += currentPreset.odGain + " ";
      stringOutput += currentPreset.volume;
      if (!currentPreset.superDistortion) {
        stringOutput += " f";
      } else {
        stringOutput += " t";
      }
      return stringOutput;
    },
    updatePreset() {
      let saveInfo =
        "u" +
        (this.activePreset + 1).toString() +
        ":" +
        this.presetString(this.activePreset);
      invoke("save_preset", { saveInfo: saveInfo });
      this.saveBool = true;
    },
    savePreset() {
      let saveInfo = "s" + (this.activePreset + 1).toString();
      invoke("save_preset", { saveInfo: saveInfo });
    },
    toggleDistortion() {
      this.superDistortion = !this.superDistortion;
    },
  },
  data() {
    return {
      saveBool: true,
      activePreset: 0,
      presetTemp: [],
      customColor: {
        backgroundColor: "Red",
      },
      superDistortion: false,
      presets: [
        {
          lpf: 0,
          // hpf: 0,
          // gateThreshold: 0,
          // compThreshold: 0,
          odGain: 0,
          volume: 0,
          superDistortion: false,
        },
        {
          lpf: 0,
          // hpf: 0,
          // gateThreshold: 0,
          // compThreshold: 0,
          odGain: 0,
          volume: 0,
          superDistortion: false,
        },
        {
          lpf: 0,
          // hpf: 0,
          // gateThreshold: 0,
          // compThreshold: 0,
          odGain: 0,
          volume: 0,
          superDistortion: false,
        },
      ],
    };
  },
};
</script>
<script setup></script>

<template>
  <!-- <link href="../dist/output.css" rel="stylesheet"> -->
  <div
    :class="{
      pre3: activePreset == 2,
      pre2: activePreset == 1,
      pre1: activePreset == 0,
    }"
    style="position: fixed, width: 100vw; height: 100vh"
  >
    <div class="container" style="position: relative; max-height: 100vh">
      <h1>Preset {{ activePreset + 1 }}</h1>
      <button
        @click="decrementActivePreset()"
        style="margin: 0 auto; margin-bottom: 30px; width: 90%"
      >
        ▲
      </button>
      <div style="margin: 0 auto; width: 80%">
        Tone:
        <!-- {{ presets[activePreset].lpf }} -->
        <vue-slider
          ref=" slider"
          v-model="presets[activePreset].lpf"
          v-on:change="saveBool = false"
        />
        <!-- HighPassCutoff:{{ 20000 - presets[activePreset].hpf * 100 + "hz" }}
        <vue-slider
          ref="slider"
          v-model="presets[activePreset].hpf"
          v-on:change="saveBool = false"
        /> -->
        <!-- gateThreshold:{{ presets[activePreset].gateThreshold }}
        <vue-slider
          ref="slider"
          v-model="presets[activePreset].gateThreshold"
          v-on:change="saveBool = false"
        /> -->
        <!-- compThreshold:{{ presets[activePreset].compThreshold }}
        <vue-slider
          ref="slider"
          v-model="presets[activePreset].compThreshold"
          v-on:change="saveBool = false"
        /> -->
        Drive:
        <!-- {{ presets[activePreset].odGain }} -->
        <vue-slider
          ref="slider"
          v-model="presets[activePreset].odGain"
          v-on:change="saveBool = false"
        />
        Volume:
        <!-- {{ presets[activePreset].volume }} -->
        <vue-slider
          ref="slider"
          v-model="presets[activePreset].volume"
          v-on:change="saveBool = false"
        />
      </div>
      Super Distortion :
      <label class="switch" style="margin: 5px 0px; margin-left: 46vw">
        <input
          type="checkbox"
          v-model="presets[activePreset].superDistortion"
          v-on:change="saveBool = false"
        />
        <span class="slider round"></span>
      </label>

      <button
        @click="incrementActivePreset()"
        style="margin: 0 auto; margin-bottom: 30px; width: 90%"
      >
        ▼
      </button>

      <!-- one button: if anything is changed (preset or value, flip to update preset button)
    if preset is updated, then chhange to save preset button -->
      <button
        @click="updatePreset()"
        v-if="!saveBool"
        class="text-3xl font-bold underline"
        style="width: 80%; margin: 0 auto; margin-top: "
      >
        Update Preset
      </button>

      <button
        @click="savePreset()"
        v-if="saveBool"
        class="text-3xl font-bold underline"
        style="width: 80%; margin: 0 auto; margin-top: "
      >
        Save Preset
      </button>
    </div>
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

.pre3 {
  background-color: rgb(104, 50, 50);
}

.pre2 {
  background-color: rgb(172, 135, 67);
}

.pre1 {
  background-color: rgb(44, 98, 44);
}
/* The switch - the box around the slider */
.switch {
  position: relative;
  display: inline-block;
  width: 60px;
  height: 34px;
}

/* Hide default HTML checkbox */
.switch input {
  opacity: 0;
  width: 0;
  height: 0;
}

/* The slider */
.slider {
  position: absolute;
  cursor: pointer;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: #ccc;
  -webkit-transition: 0.4s;
  transition: 0.4s;
}

.slider:before {
  position: absolute;
  content: "";
  height: 26px;
  width: 26px;
  left: 4px;
  bottom: 4px;
  background-color: white;
  -webkit-transition: 0.4s;
  transition: 0.4s;
}

input:checked + .slider {
  background-color: #2196f3;
}

input:focus + .slider {
  box-shadow: 0 0 1px #2196f3;
}

input:checked + .slider:before {
  -webkit-transform: translateX(26px);
  -ms-transform: translateX(26px);
  transform: translateX(26px);
}

/* Rounded sliders */
.slider.round {
  border-radius: 34px;
}

.slider.round:before {
  border-radius: 50%;
}
html,
body {
  margin: 0px !important;
  padding: 0px !important;
}
</style>
