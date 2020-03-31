<template>
  <div class="rise-up-box">
    <div v-for="(item, index) in content"
         class="row"
         :key="index">
      <div class="row-text" :class="classList[index]" :style="{'--animate-duration':duration}">
        <div :style="{'background':background}" class="bg-content pre-wrap">{{item}}</div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'Caption_1',
  props: {
    content: Array,
    isOnce: [String, Boolean, Number],
    background: String,
    animateRate: [String, Number]
  },
  data () {
    return {
      classList: [],
      timer: 0,
      duration: null
    }
  },
  mounted () {
    this.readyAnimate()
  },
  methods: {
    readyAnimate () {
      let defaultDuration = 450
      this.duration = (this.content.length + 1) * defaultDuration * this.animateRate + 'ms'
      this.classList = []
      this.timer = setInterval(() => {
        this.classList.push('animation-in')
        if (this.classList.length > this.content.length) {
          clearInterval(this.timer)
          !this.isOnce && this.animateOut()
        }
      }, 150)
    },
    animateOut () {
      setTimeout(() => {
        this.classList = new Array(this.content.length).fill('animation-out')
        setTimeout(() => {
          this.readyAnimate()
        }, 500)
      }, 2500)
    }
  }
}
</script>

<style scoped lang="scss">
  .rise-up-box {
    padding: 20px;
    overflow: hidden;
    --animate-duration: 3000ms;
  }

  .row {
    overflow: hidden;
    margin-bottom: 3px;
  }

  .row-text {
    word-break: break-word;
    white-space: normal;
    max-width: 100%;
    transform: translate3d(0, 20px, 0) rotate(5deg);
    opacity: 0;

    &.animation-in {
      animation-name: AnimateIn;
      transform-origin: 0 0;
      animation-fill-mode: both;
      animation-duration: var(--animate-duration);
    }

    &.animation-out {
      transform: translate3d(0, 0, 0) rotate(0);
      animation: fadeOut 0.3s linear both;
    }
  }

  @keyframes AnimateIn {
    0% {
      opacity: 0;
      transform: translate3d(0, 20px, 0) rotate(5deg);
    }
    20%, 100% {
      opacity: 1;
      transform: translate3d(0, 0, 0) rotate(0);
    }
  }
</style>
