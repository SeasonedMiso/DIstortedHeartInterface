<script setup>
import appInterface from "./components/appInterface.vue";
import connectArduinoMsg from "./components/connectArduinoMsg.vue";
import testPortMessage from "./components/testPortMessage.vue";
import { invoke } from "@tauri-apps/api/tauri";
</script>

<script>
export default {
  name: "App",
  components: {
    appInterface,
    connectArduinoMsg,
  },
  data() {
    return {
      polling: null,
      arduinoState: "0",
    };
  },
  methods: {
    sleep(ms) {
      return new Promise((resolve) => setTimeout(resolve, ms));
    },
    pollArduino() {
      this.polling = setInterval(() => {
        invoke("arduino_found").then((message) => {
          this.arduinoState = message;
          // console.log(this.arduinoState)
        });
      }, 1000);
    },
  },
  beforeDestroy() {
    clearInterval(this.polling);
  },
  created() {
    this.pollArduino();
  },
};
</script>
<template>
  <!-- <testPortMessage v-if="arduinoState === '1'" /> -->
  <appInterface v-if="arduinoState === '1'" />
  <connectArduinoMsg v-if="arduinoState === '0'" />
</template>
