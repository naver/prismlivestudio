<template>
  <div @animationend="animationend" ref="$container" v-if="isShow" class="animation-box" :class="className">
    <div class="pre-wrap min-height-row row" v-for="(item, index) in content" :style="alignStyle" :key="index">
      <span v-for="(char, subIndex) in item" :key="subIndex" class="char" :class="{'empty':char.trim()===''}" :style="style">{{char}}</span>
    </div>
  </div>
</template>

<script>
export default {
  name: 'element-2',
  props: {
    content: Array,
    background: {
      type: String,
      default: '#50E3C2'
    },
    isOnce: [String, Boolean, Number],
    align: [String, Boolean, Number],
    animateRate: [String, Number]
  },
  data () {
    return {
      isShow: false,
      className: ''
    }
  },
  computed: {
    alignStyle () {
      return {'justifyContent': ({'left': 'flex-start', 'center': 'center', 'right': 'flex-end'})[this.align] || 'center'}
    },
    style () {
      return {
        backgroundColor: this.background || '#50E3C2',
        ...this.durationStyle
      }
    },
    durationStyle () {
      let defaultDuration = 300
      return {
        '--animation-duration': (defaultDuration * this.animateRate) + 'ms'
      }
    }
  },
  mounted () {
    this.className = 'animation-in'
  },
  watch: {
    content: {
      handler (now) {
        this.isShow = false
        this.$nextTick(() => {
          this.isShow = true
        })
      },
      deep: true,
      immediate: true
    }
  },
  methods: {
    animationend () {
      let classList = this.$refs.$container.classList
      if (classList.contains('animation-in')) {
        this.animationInEnd()
      } else if (classList.contains('animation-out')) {
        this.animationOutEnd()
      }
    },
    animationInEnd () {
      if (!this.isOnce) {
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
  .animation-box {
    &.animation-in {
      .char {
        animation: AnimateIn var(--animation-duration) cubic-Bezier(0.65, 0, 0.4, 1.4) both;
      }
    }

    &.animation-out {
      .char {
        animation: AnimateOut var(--animation-duration) cubic-Bezier(0.65, 0, 0.4, 1.4) both;
      }
    }
  }

  .row {
    display: flex;
    justify-content: center;
    flex-wrap: wrap;
  }

  .char {
    display: inline-block;
    padding: 4px;
    line-height: 1;
    border-radius: 3px;
    border: 1px solid #000000;
    text-shadow: 2px 2px #0c5746;

    &:not(:last-child) {
      margin-right: 2px;
    }
  }

  .empty {
    border-color: transparent !important;
    background: transparent !important;
  }

  @keyframes AnimateIn {
    0% {
      transform: scale3d(0, 0, 0);
      opacity: 1;
    }
    33.3% {
      transform: scale3d(1.15, 1.15, 1);
      opacity: 1;
    }
    66.6% {
      transform: scale3d(.95, .95, 1);
      opacity: 1;
    }
    100% {
      transform: scale3d(1, 1, 1);
      opacity: 1;
    }
  }

  @keyframes AnimateOut {
    0% {
      transform: scale3d(1, 1, 1);
      opacity: 1;
    }
    100% {
      transform: scale3d(0, 0, 0);
      opacity: 0;
    }
  }
</style>
