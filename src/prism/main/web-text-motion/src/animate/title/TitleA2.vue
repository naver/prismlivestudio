<template>
  <div ref="$animate" @animationend="animationend" :class="className">
    <div class="box">
      <div :style="aniStyle" class="up-slide wrap-center">
        <div :style="{'background':data.background}" class="bg-content">
          <div class="min-height-row pre-wrap" v-for="(item,index) in data.content" :key="index">{{item}}</div>
        </div>
      </div>
    </div>
    <div class="box">
      <div :style="aniStyle" class="down-slide wrap-center">
        <div :style="{'background':data.background}" class="bg-content">
          <div class="min-height-row pre-wrap" v-for="(item,index) in data.subContent" :key="index">{{item}}</div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>

export default {
  name: 'Title_A2',
  props: ['data'],
  data () {
    return {
      className: ''
    }
  },
  mounted () {
    this.className = 'animation-in'
  },
  computed: {
    aniStyle () {
      let defaultDuration = 480
      return {
        'textAlign': this.data.align,
        '--animation-duration': (defaultDuration * this.data.animateRate) + 'ms'
      }
    }
  },
  methods: {
    animationend () {
      this.$refs.$animate.classList.contains('animation-in') ? this.animationInEnd() : this.animationOutEnd()
    },
    animationInEnd () {
      if (!this.data.isOnce) {
        setTimeout(() => {
          this.className = 'animation-out'
        }, 1600)
      }
    },
    animationOutEnd () {
      setTimeout(() => {
        this.className = 'animation-in'
      })
    }
  }
}
</script>

<style scoped lang="scss">
  .box {
    width: 100%;
    overflow: hidden;
  }

  .up-slide {
    white-space: normal;
    animation-fill-mode: both;
    animation-duration: var(--animation-duration);
    animation-timing-function: cubic-bezier(0.3, 0, 0.1, 1);
  }

  .down-slide {
    white-space: normal;
    animation-fill-mode: both;
    animation-duration: var(--animation-duration);
    animation-timing-function: cubic-bezier(0.3, 0, 0.1, 1);
  }

  .animation-in {
    .up-slide {
      animation-name: UpIn;
    }

    .down-slide {
      animation-name: DownIn;
    }
  }

  .animation-out {
    .up-slide {
      animation-name: UpOut;
    }

    .down-slide {
      animation-name: DownOut;
    }
  }

  @keyframes UpIn {
    0% {
      transform: translate(0, 100%);
    }
    100% {
      transform: translate(0, 0);
    }
  }

  @keyframes UpOut {
    0% {
      transform: translate(0, 0);
    }
    100% {
      transform: translate(0, 100%);
    }
  }

  @keyframes DownIn {
    0% {
      transform: translate(0, -100%);
    }
    100% {
      transform: translate(0, 0);
    }
  }

  @keyframes DownOut {
    0% {
      transform: translate(0, 0);
    }
    100% {
      transform: translate(0, -100%);
    }
  }
</style>
