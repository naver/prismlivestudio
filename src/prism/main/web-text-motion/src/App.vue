<template>
  <div id="app">
    <Test v-if="isDebugger"/>
    <MainView v-else/>
  </div>
</template>

<script>
import MainView from './components/MainView'
import Test from './test/Test'

export default {
  name: 'App',
  components: {
    MainView,
    Test
  },
  data () {
    let isDebugger = !!new URLSearchParams(document.location.search).get('debugger')
    return {
      isDebugger: isDebugger
    }
  },
  mounted () {
    this.isDebugger && (document.getElementById('app').style.overflow = 'scroll')
  }
}
</script>

<style>
  #app, body, html {
    width: 100%;
    height: 100%;
    margin: 0;
    padding: 0;
  }

  #app {
    overflow: hidden;
    display: flex;
    align-items: center;
    justify-content: center;
  }
</style>
