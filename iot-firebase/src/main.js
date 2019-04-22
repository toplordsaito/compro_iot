import Vue from 'vue'
import App from './App.vue'
import Bootstrap from 'bootstrap/dist/css/bootstrap.min.css'
import firebase from 'firebase'

Vue.config.productionTip = false

const config = {
  apiKey: "AIzaSyCFfP5Gmv9_YL3VO_282WXPyHrxGLrg3Vo",
  authDomain: "iot-itcamp-4faf6.firebaseapp.com",
  databaseURL: "https://iot-itcamp-4faf6.firebaseio.com",
  projectId: "iot-itcamp-4faf6",
  storageBucket: "iot-itcamp-4faf6.appspot.com",
  messagingSenderId: "507261967577"
};
firebase.initializeApp(config)

new Vue({
  render: h => h(App),
  Bootstrap,
}).$mount('#app')
