<script>
import { invoke } from "@tauri-apps/api/tauri";
export default {
  name: "testPortMessage",
  setup() {},
  methods: {
    testMessage() {
      let testMsg = "";
      console.log("Sending msg to arduino");
      invoke("send_test_msg", { testMsg: this.textBox });
    },
    // breakPortConnection() {
    //   let testMsg = "";
    //   console.log("Sending msg to arduino");
    //   invoke("breakPort", { testMsg: this.textBox });
    // },
  },
  data() {
    return {
      textBox: "",
    };
  },
};
</script>
<template>
  <input v-model="textBox" placeholder="0" />
  <button
    @click="testMessage()"
    class="text-3xl font-bold underline"
    style="width: 80%; margin: 0 auto; margin-top: "
  >
    Send Test String
  </button>
</template>
