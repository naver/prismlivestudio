<template>
  <div>
    <div ref="$container" class="animation-box">
      <div ref="$wrapper" class="wrapper" :style="rowStyle" :class="extendClass">
        <div class="min-height-row pre-wrap row"
             v-for="(item, index) in content"
             :style="durationStyle"
             :key="index"
             :class="extendClass">{{item}}</div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'element-1',
  props: {
    content: Array,
    background: String,
    isOnce: [String, Boolean, Number],
    animateRate: [String, Number],
    align: [String, Number]
  },
  computed: {
    rowStyle () {
      return {
        'textAlign': this.align,
        '--light-bg': this.background || '#FF4D4D',
        ...this.extendStyle,
        ...this.durationStyle
      }
    },
    extendClass () {
      return this.isOnce ? 'is-once' : 'is-infinite'
    },
    durationStyle () {
      let defaultDuration = 2900
      return {
        '--animation-duration': (defaultDuration * this.animateRate) + 'ms'
      }
    }
  },
  data () {
    return {
      extendStyle: {},
      className: ''
    }
  },
  methods: {
    setHeight () {
      let h = this.$refs.$wrapper.offsetWidth - 24
      this.extendStyle = {
        height: h + 'px',
        lineHeight: h + 'px',
        borderRadius: '50%'
      }
    }
  }
}
</script>

<style scoped lang="scss">
  .animation-box {
    font-weight: bold;
    padding: 32px;
  }

  .wrapper {
    text-align: center;
    position: relative;
    transform: rotate(-10deg);
    border-radius: 3px;
    overflow: hidden;
    padding: 12px 24px;
    --light-bg: #FF4D4D;
    --animate-bg1: ani_bg1;
    --animate-bg2: ani_bg2;
    --animate-count: infinite;

    &::before {
      content: '';
      position: absolute;
      top: 50%;
      left: 50%;
      width: 200%;
      background-color: var(--light-bg);
      background-repeat: no-repeat;
      border-radius: 50%;
      padding-top: 200%;
      z-index: 2;
      height: 100%;
    }

    &.is-once::before {
      animation: ani_bg1_once var(--animation-duration) both;
    }

    &.is-infinite::before {
      animation: ani_bg1 var(--animation-duration) infinite both;
    }

    &::after {
      content: '';
      position: absolute;
      top: 50%;
      left: 50%;
      width: 200%;
      background-color: #ffffff;
      background-repeat: no-repeat;
      border-radius: 50%;
      padding-top: 200%;
      animation: ani_bg2 var(--animation-duration) infinite both;
      height: 100%;
    }

    &.is-once::after {
      animation: ani_bg2_once var(--animation-duration) both;
    }

    &.is-infinite::after {
      animation: ani_bg2 var(--animation-duration) infinite both;
    }

  }

  .row {
    white-space: pre-wrap;
    word-break: break-word;
    position: relative;
    z-index: 3;

    &.is-once {
      animation: ani_text1 var(--animation-duration) cubic-bezier(0, 0, 0, 1) both, ani_text2_once var(--animation-duration) linear both;
    }

    &.is-infinite {
      animation: ani_text1 var(--animation-duration) cubic-bezier(0, 0, 0, 1) infinite both, ani_text2 var(--animation-duration) infinite linear both;
    }
  }

  @keyframes ani_bg1 {
    0%, 4% {
      animation-timing-function: cubic-Bezier(4, 0, .2, 1);
      transform: translate3d(-50%, -50%, 0) scale3d(0, 0, 1);
    }
    30%, 78% {
      transform: translate3d(-50%, -50%, 0) scale3d(1, 1, 1);
      animation-timing-function: cubic-Bezier(.8, 0, .6, 1);
    }
    98%, 100% {
      transform: translate3d(-50%, -50%, 0) scale3d(0, 0, 1);
    }
  }

  @keyframes ani_bg1_once {
    0%, 4% {
      animation-timing-function: cubic-Bezier(4, 0, .2, 1);
      transform: translate3d(-50%, -50%, 0) scale3d(0, 0, 1);
    }
    30%, 100% {
      transform: translate3d(-50%, -50%, 0) scale3d(1, 1, 1);
      animation-timing-function: cubic-Bezier(.8, 0, .6, 1);
    }
  }

  @keyframes ani_bg2 {
    0% {
      transform: translate3d(-50%, -50%, 0) scale3d(0, 0, 1);
      animation-timing-function: cubic-Bezier(4, 0, .2, 1);
    }
    35%, 78% {
      transform: translate3d(-50%, -50%, 0) scale3d(1, 1, 1);
      animation-timing-function: cubic-Bezier(.8, 0, .6, 1);
    }
    100% {
      transform: translate3d(-50%, -50%, 0) scale3d(0, 0, 1);
    }
  }

  @keyframes ani_bg2_once {
    0% {
      transform: translate3d(-50%, -50%, 0) scale3d(0, 0, 1);
      animation-timing-function: cubic-Bezier(4, 0, .2, 1);
    }
    35%, 100% {
      transform: translate3d(-50%, -50%, 0) scale3d(1, 1, 1);
      animation-timing-function: cubic-Bezier(.8, 0, .6, 1);
    }
  }

  @keyframes ani_text1 {
    0%, 8% {
      transform: translateY(60px);
    }
    15%, 100% {
      transform: translateY(0);
    }
  }

  @keyframes ani_text2 {
    0%, 8% {
      opacity: 0;
    }
    15%, 85% {
      opacity: 1;
    }
    92%, 100% {
      opacity: 0;
    }
  }

  @keyframes ani_text2_once {
    0%, 8% {
      opacity: 0;
    }
    15%, 85% {
      opacity: 1;
    }
  }
</style>
