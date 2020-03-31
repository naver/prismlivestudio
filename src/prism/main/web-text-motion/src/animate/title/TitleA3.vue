<template>
  <div
    ref="$container"
    class="animate-box wrap-center"
    :class="className"
    @animationend="animationend"
    :style="boxStyle"
  >
    <div class="animate-content" :style="contentStyle">
      <div :style="durationStyle" class="content-word">
        <div class="min-height-row pre-wrap" v-for="(item,index) in data.content" :key="index">{{item}}</div>
      </div>
    </div>
    <div
      ref="$content" :style="{'width':contentStyle['min-width']}"
      style="position:absolute;visibility: hidden;">
      <div class="min-height-row pre-wrap" v-for="(item,index) in data.content" :key="index">{{item}}</div>
    </div>
    <div ref="$content2" style="white-space: nowrap;visibility: hidden;">
      <div class="min-height-row pre-wrap" v-for="(item,index) in data.content" :key="index">{{item}}</div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'Title_A3',
  props: ['data'],
  data () {
    return {
      boxWidth: 'auto',
      boxHeight: 'auto',
      parentWidth: 0,
      className: ''
    }
  },
  computed: {
    contentWidth () {
      return -this.boxWidth / 2 + 'px'
    },
    contentStyle () {
      return {'min-width': Math.min(this.parentWidth - 70, this.boxWidth + 2) + 'px'}
    },
    boxStyle () {
      return {
        ...this.durationStyle,
        'textAlign': this.data.align,
        'backgroundColor': this.data.background || 'white',
        '--box-width': Math.min(this.parentWidth, this.boxWidth + 70) + 'px',
        'height': (this.boxHeight + 30) + 'px'
      }
    },
    durationStyle () {
      let defaultDuration = 620
      return {
        '--animation-duration': (defaultDuration * this.data.animateRate) + 'ms'
      }
    }
  },
  mounted () {
    this.boxWidth = this.$refs.$content2.offsetWidth
    this.parentWidth = this.$refs.$container.parentElement.offsetWidth
    this.$nextTick(() => {
      setTimeout(() => {
        this.boxHeight = this.$refs.$content.offsetHeight
        this.className = 'animation-in'
      })
    })
  },
  methods: {
    animationend (el) {
      this.$refs.$container.classList.contains('animation-in') ? this.animationInEnd() : this.animationOutEnd()
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
  .animate-box {
    --box-width: 0px;
    width: var(--box-width);
    position: relative;
    box-sizing: border-box;
    overflow: hidden;;
    animation-fill-mode: both;
    animation-duration: var(--animation-duration);
    animation-timing-function: cubic-bezier(0.4, 0, 0.1, 1);
  }

  .content-word {
    transform: translate3d(0, -50%, 0);
    opacity: 0;
    animation-fill-mode: both;
    animation-duration: var(--animation-duration);
    animation-timing-function: cubic-bezier(0.1, 0, 0.5, 1);
  }

  .animate-content {
    white-space: normal;
    word-break: break-word;
    position: absolute;
    top: 50%;
    left: 50%;
    transform: translate3d(-50%, -50%, 0);
  }

  .animation-in {
    &.animate-box {
      animation-name: BoxShow;
    }

    .content-word {
      animation-name: ContentShow;
    }
  }

  .animation-out {
    &.animate-box {
      animation-name: BoxHide;
    }

    .content-word {
      animation-name: ContentHide;
    }
  }

  @keyframes BoxShow {
    0% {
      width: 0;
    }
    100% {
      width: var(--box-width);
    }
  }

  @keyframes BoxHide {
    0% {
      width: var(--box-width);
    }
    100% {
      width: 0;
    }
  }

  @keyframes ContentShow {
    0% {
      opacity: 0;
      transform: translate3d(0, -50%, 0);
    }
    100% {
      opacity: 1;
      transform: translate3d(0, 0, 0);
    }
  }

  @keyframes ContentHide {
    0% {
      opacity: 1;
      transform: translate3d(0, 0, 0);
    }
    100% {
      opacity: 0;
      transform: translate3d(0, -50%, 0);
    }
  }
</style>
