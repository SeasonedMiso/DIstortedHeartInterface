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
  // methods: {
  //   test() {
  //     window.ipcRenderer.send("arduinoTest", "arg here") // or any other ipcRenderer method you want to invoke
  //     console.log(window.ipcRenderer)
  //   }
  // },
  data() {
    return {
      slider1: 0,
      slider2: 0
    }
  }
};
</script>
<script setup>
</script>

<template>
  <div class="container">
    slider1:{{ slider1 }}
    <vue-slider ref="slider" v-model="slider1" />
    slider2:{{ slider2 }}
    <vue-slider ref="slider" v-model="slider2" />
    <vue-slider ref="slider" v-model="slider2" />

    <button @click="test" class="text-3xl font-bold underline">'ello</button>

  </div>
</template>

<style scoped>
.logo.vite:hover {
  filter: drop-shadow(0 0 2em #747bff);
}

.logo.vue:hover {
  filter: drop-shadow(0 0 2em #249b73);
}
</style>
