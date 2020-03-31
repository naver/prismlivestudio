<template>
  <div :style="aniStyle"
       @animationend="animationend"
       ref="$animate" :class="className"
       class="content wrap-center">
    <div :style="{'background':data.background,'textAlign':data.align}" class="bg-content">
      <div class="min-height-row pre-wrap" v-for="(item,index) in data.content" :key="index">{{item}}</div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'TitleA1',
  props: ['data'],
  data () {
    return {
      className: ''
    }
  },
  computed: {
    aniStyle () {
      let defaultDuration = 1000
      return {
        '--animation-duration': (defaultDuration * this.data.animateRate) + 'ms'
      }
    }
  },
  mounted () {
    this.className = 'animation-in'
  },
  methods: {
    animationend () {
      this.$refs.$animate.classList.contains('animation-in') ? this.animationInEnd() : this.animationOutEnd()
    },
    animationInEnd () {
      if (!this.data.isOnce) {
        setTimeout(() => {
          this.className = 'animation-out'
        }, 2000)
      }
    },
    animationOutEnd () {
      setTimeout(() => {
        this.className = 'animation-in'
      }, 100)
    }
  }
}
</script>

<style scoped>
  .content {
    white-space: normal;
    animation-timing-function: linear;
    animation-duration: var(--animation-duration);
    animation-fill-mode: both;
  }

  .animation-in {
    animation-name: fadeIn;
  }

  .animation-out {
    animation-name: fadeOut;
  }

</style>
