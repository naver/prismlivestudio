<template>
  <div ref="$box" style="position: relative;width: 100%;height: 100%;">
    <div ref="$temp" style="visibility:hidden;position:absolute;display: flex;flex-direction: column;">
      <div style="letter-spacing: 2px;">{{data.content.join('')}}</div>
      <div style="letter-spacing: 2px;" :style="subStyle">{{data.subContent.join('')}}</div>
    </div>
    <div ref="$container" @animationend="animationend" :style="speedStyle" class="box-animate" :class="className">
      <div class="line-box" :style="{'--border-color':this.data.fontColor||'white'}">
        <div>
          <div :style="speedStyle" class="text-content wrap-center">
            <div :style="{'background':data.background}">
              <div class="min-height-row pre-wrap" v-for="(item,index) in data.content" :key="index">{{item}}</div>
            </div>
          </div>
          <div :style="subStyle" class="text-sub wrap-center">
            <div :style="{'background':data.background}">
              <div class="min-height-row pre-wrap" v-for="(item,index) in data.subContent" :key="index">{{item}}</div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'LowerThird1',
  props: ['data'],
  data () {
    return {
      className: ''
    }
  },
  computed: {
    subStyle () {
      let size = this.data.fontSize || ''
      if (!size) return {}
      let num = parseInt(size)
      let unit = size.substring((num + '').length)
      return {
        fontSize: (num * 0.6) + unit,
        ...this.speedStyle
      }
    },
    speedStyle () {
      let defaultDuration = 1000 * this.data.animateRate
      return {
        '--duration15': defaultDuration * 1.5 + 'ms',
        '--duration8': defaultDuration * 0.8 + 'ms',
        '--duration5': defaultDuration * 0.5 + 'ms'
      }
    }
  },
  mounted () {
    this.className = 'animation-in'
    let parentW = this.$refs.$box.parentNode.offsetWidth
    this.$refs.$container.style.width = Math.min(parentW - 100, this.$refs.$temp.offsetWidth + 25) + 'px'
  },
  methods: {
    animationend () {
      let classList = this.$refs.$container.classList
      if (classList.contains('animation-in')) {
        this.className = 'animation-in-next'
      } else if (classList.contains('animation-in-next')) {
        this.animationInEnd()
      } else if (classList.contains('animation-out')) {
        this.animationOutEnd()
      }
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

<style scoped lang="scss">
  .line-box {
    border-left: var(--border-color) 3px solid;
    padding-left: 20px;
    overflow: hidden;
  }

  .animate-content {
    word-break: break-word;
    white-space: normal;
    width: 100%;
  }

  .box-animate {
    position: absolute;
    display: flex;
    top: 50%;
    left: 50%;
    transform: translate3d(0, -50%, 0) scaleY(0);

    .text-content {
      margin-bottom: 6px;
      position: relative;
      letter-spacing: 2px;
      text-align: left;
      visibility: hidden;
    }

    .text-sub {
      visibility: hidden;
      position: relative;
      text-align: left;
      letter-spacing: 2px;
      font-size: 16px;
    }

    &.animation-in {
      animation: BoxOpen var(--duration5) cubic-bezier(0, 0, 0.4, 1) both;
    }

    &.animation-in-next {
      animation: BoxAnimateIn var(--duration8) cubic-bezier(0.6, 0, 0.2, 1) both;

      .text-content {
        animation: contentAnimateIn var(--duration8) cubic-bezier(0, 0, 0.1, 1) both;
      }

      .text-sub {
        animation: contentAnimateIn var(--duration8) cubic-bezier(0, 0, 0.4, 1) both;
      }
    }

    &.animation-out {
      animation: BoxAnimateOut var(--duration8) cubic-bezier(0.6, 0, 0.2, 1) both;

      .text-content {
        animation: contentAnimateOut var(--duration15) cubic-bezier(0.6, 0, 0.2, 1) both;
      }

      .text-sub {
        animation: contentAnimateOut var(--duration15) cubic-bezier(0.6, 0, 0.2, 1) both;
      }
    }
  }

  @keyframes BoxOpen {
    0% {
      transform: translate3d(50%, -50%, 0) scaleY(0);
    }
    100% {
      transform: translate3d(50%, -50%, 0) scaleY(1);
    }
  }

  @keyframes BoxAnimateIn {
    0% {
      transform: translate3d(50%, -50%, 0) scaleY(1);
    }
    100% {
      transform: translate3d(-50%, -50%, 0) scaleY(1);
    }
  }

  @keyframes BoxAnimateOut {
    0% {
      transform: translate3d(-50%, -50%, 0) scaleY(1);
    }
    66.6% {
      transform: translate3d(50%, -50%, 0) scaleY(1);
      opacity: 1;
    }
    100% {
      transform: translate3d(50%, -50%, 0) scaleY(1);
      opacity: 0;
    }
  }

  @keyframes contentAnimateIn {
    0% {
      visibility: hidden;
      left: -120%;
    }
    33.3% {
      visibility: hidden;
      left: -120%;
    }
    100% {
      visibility: visible;
      left: 0;
    }
  }

  @keyframes contentAnimateOut {
    0% {
      visibility: visible;
      left: 0;
    }
    33% {
      left: -120%;
      visibility: visible;
    }
    100% {
      visibility: hidden;
      left: -120%;
    }
  }
</style>
