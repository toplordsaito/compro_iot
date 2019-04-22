<template>
  <div id="app">
    <h1>Dashboard</h1>
    <div class="container">
      <div class="row">
        <div class="col">
          <hr>
        </div>
      </div>
      <div class="row">
        <data-card
        v-for="data in dataCollection"
        :key="data.name"
        :name="data.name"
        :value="data.value"
        />
      </div>
      <div class="row">
        <control-card
        @isClick="triggerState(device)"
        v-for="device in deviceCollection"
        :key="device.name"
        :name="device.name" 
        :isOn="device.isOn" 
        :image="device.image"/>
      </div>
    </div>
  </div>
</template>

<script>
import { database } from 'firebase'
import DataCard from '@/components/DataCard'
import ControlCard from '@/components/ControlCard'
export default {
  name: 'app',
  components: {
    DataCard,
    ControlCard
  },
  data () {
    return {
      dataCollection: [
        {name: 'Temperature', value: 20},
        {name: 'Time', value: "13:00"},
        {name: 'Status', value: "Connected"}
      ],
      deviceCollection: [
        {name: 'LED1', isOn: false, image: require('./assets/fan.png')},
        {name: 'LED2', isOn: true, image: require('./assets/light.png')}
      ]
    }
  },
  methods: {
    triggerState (device) {
      device.isOn = !device.isOn
      database().ref(device.name).set(device.isOn)
    }
  }
}
</script>

<style lang="scss">
#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin: 60px 0;
}
</style>
