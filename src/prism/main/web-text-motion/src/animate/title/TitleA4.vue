<template>
  <div :style="durationStyle" ref="$animate" @animationend="animationend" class="animation-box" :class="className">
    <div class="title">
      <div class="wrap-center" :style="alignStyle">
        <div :style="{'background':data.background}" class="bg-content">
          <div class="min-height-row pre-wrap" v-for="(item,index) in data.content" :key="index">{{item}}</div>
        </div>
        <div class="line">{{'/'}}</div>
        <div v-if="data.subContent" :style="subStyle" class="subtitle wrap-style">
          <div :style="{'background':data.background}" class="bg-content">
            <div class="min-height-row pre-wrap" v-for="(item,index) in data.subContent" :key="index">{{item}}</div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>

export default {
  name: 'Title_A4',
  props: ['data'],
  computed: {
    durationStyle () {
      let defaultDuration = 620
      return {
        '--animation-duration': (defaultDuration * this.data.animateRate) + 'ms'
      }
    },
    subStyle () {
      let size = this.data.fontSize || ''
      if (!size) return {}
      let num = parseInt(size)
      let unit = size.substring((num + '').length)
      return {
        fontSize: (num * 3 / 10) + unit,
        ...this.alignStyle
      }
    },
    alignStyle () {
      return {
        textAlign: this.data.align
      }
    }
  },
  data () {
    return {
      className: ''
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

<style scoped lang="scss">
  .animation-box {
    width: 100%;
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    animation-fill-mode: both;
    animation-duration: var(--animation-duration);
    animation-timing-function: linear;

    &.animation-in {
      animation-name: fadeIn;
    }

    &.animation-out {
      animation-name: fadeOut;
    }
  }

  .title {
    display: flex;
    flex-direction: column;
    align-items: center;
  }

  .line {
    font-size: 28px;
    font-weight: normal;
    margin-bottom: 10px;
    padding: 0 15px;
  }

  .subtitle {
    font-size: 18px;
    letter-spacing: 2px;
  }
</style>
