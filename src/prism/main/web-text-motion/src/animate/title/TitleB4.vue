<template>
  <div ref="el" :style="{'background':data.background}">
    <div v-for="(wItem,wIndex) in words" :style="alignStyle" :key="wIndex" class="pre-wrap min-height-row wrap-center animate-box">
      <div
        ref="$word"
        v-for="(item,key) in wItem"
        v-if="item!=='\\n'"
        :style="{...animateStyle,...delayStyle(),'width':item===' '?maxWordWidth:'auto'}"
        :key="key"
        :class="{'animate-sway':!data.isOnce}">{{item}}</div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'TitleB4',
  props: ['data'],
  data () {
    return {
      isShow: false,
      maxWordWidth: 'auto'
    }
  },
  watch: {
    words: {
      handler (now) {
        this.$nextTick(() => {
          this.getMaxWordWidth()
        })
      },
      immediate: true
    }
  },
  computed: {
    alignStyle () {
      return {'justifyContent': ({'left': 'flex-start', 'center': 'center', 'right': 'flex-end'})[this.data.align] || 'center'}
    },
    words () {
      let result = []
      this.data.content.forEach(item => {
        result.push(item.split(''))
      })
      return result
    },
    animateStyle () {
      let defaultDuration = 200 * this.data.animateRate
      return {
        '--animation-duration': defaultDuration + 'ms'
      }
    }
  },
  methods: {
    getMaxWordWidth () {
      let list = this.$refs.$word
      let maxWidth = 0
      list.forEach(item => {
        item.offsetWidth > maxWidth && (maxWidth = item.offsetWidth)
      })
      this.maxWordWidth = maxWidth + 'px'
    },
    delayStyle () {
      let i = parseInt(this.animateStyle['--animation-duration'])
      return {
        'animationDelay': Math.random().toFixed(2) * i + 'ms'
      }
    }
  }
}
</script>

<style scoped>
  .animate-box {
    display: flex;
    width: auto;
    justify-content: center;
    flex-wrap: wrap;
  }

  .animate-sway {
    margin: 4px;
    animation-duration: var(--animation-duration);
    animation-fill-mode: both;
    animation-name: Sway;
    animation-iteration-count: infinite;
  }

  @keyframes Sway {
    0% {
      transform: scale(1, 1);
    }
    50% {
      transform: scale(1.2, 1.2);
    }
    100% {
      transform: scale(1, 1);
    }
  }
</style>
