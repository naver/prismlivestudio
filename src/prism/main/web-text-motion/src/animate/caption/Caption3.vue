<template>
  <div ref="$box" class="box" :style="{'alignItems':data.align==='right'?'flex-end':'flex-start'}">
    <div style="visibility: hidden" class="temp-box">
      <div v-for="(item, index) in data.content" :key="'w'+index">
        <span ref="$textList">{{item}}</span>
      </div>
    </div>
    <div style="display: flex;flex-direction: column;" :style="alignStyle">
      <div ref="$item"
           :class="{'animate-row-in':showCount>index,[className]:true}"
           :style="{...rowStyle,'min-width':widthList[index]||'auto',width:widthList[index]||'auto'}"
           class="animate-row" v-for="(item, index) in data.content" :key="index">
        <div :style="{...alignStyle,'padding':!item?'0':null}" class="animate-text">{{item}}</div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'Caption_3',
  props: ['data'],
  data () {
    return {
      showCount: 0,
      timer: 0,
      className: '',
      widthList: []
    }
  },
  computed: {
    durationNum () {
      return 400 * this.data.animateRate
    },
    rowStyle () {
      return {
        '--animation-duration': this.durationNum + 'ms',
        'background': this.data.background || 'white',
        ...this.alignStyle
      }
    },
    alignStyle () {
      return {
        'alignItems': {'left': 'flex-start', 'center': 'center', 'right': 'flex-end'}[this.data.align],
        'textAlign': this.data.align,
        '--text-translate': this.data.align === 'right' ? 'translateX(100%)' : 'translateX(-100%)',
        '--row-translate': this.data.align === 'right' ? 'translateX(120%)' : 'translateX(-120%)'
      }
    }
  },
  mounted () {
    let list = []
    let parentBoxW = this.$refs.$box.parentElement.offsetWidth - 20
    this.$refs.$textList.forEach((i, index) => {
      if (!this.data.content[index]) {
        list.push('0px')
      } else {
        list.push(Math.min(Math.ceil(i.offsetWidth + 1), parentBoxW) + 'px')
      }
    })
    this.widthList = list
    this.startShow()
  },
  methods: {
    startShow () {
      this.showCount = 0
      this.className = ''
      this.timer = setInterval(() => {
        this.showCount++
        if (this.showCount > this.data.content.length) {
          clearInterval(this.timer)
          setTimeout(() => {
            this.reset()
          }, 2000)
        }
      }, 400)
    },
    reset () {
      if (this.data.isOnce) {
        return false
      }
      this.showCount = 0
      this.className = 'animate-row-out'
      setTimeout(() => {
        this.startShow()
      }, this.durationNum + 200)
    }
  }
}
</script>

<style scoped lang="scss">
  .box {
    position: relative;
    box-sizing: border-box;
    overflow: hidden;
    display: flex;
    flex-direction: column;
  }

  .temp-box {
    position: absolute;
    top: 0;
    left: 0;
    width: 100%;
    height: 100%;

    span {
      padding: 0 4px;
    }
  }

  .animate-row {
    max-width: calc(100% - 20px);
    margin-bottom: 5px;
    transform: var(--row-translate);
    overflow: hidden;

    &.animate-row-in {
      animation: showRow var(--animation-duration) cubic-bezier(0, 0, .4, 1) both;

      .animate-text {
        animation: showRow var(--animation-duration) cubic-bezier(0, 0, .4, 1) .2s both
      }
    }

    &.animate-row-out {
      animation: hideRow var(--animation-duration) cubic-bezier(.33, 0, 0.4, 1) both;

      .animate-text {
        transform: translateX(0);
      }
    }
  }

  .animation-box {
    display: flex;
    flex-direction: column;
    align-items: flex-start;
    overflow: hidden;
  }

  .animate-text {
    padding: 0 4px;
    display: inline-block;
    max-width: 100%;
    word-break: break-all;
    white-space: pre-wrap;
    transform: var(--text-translate);
  }

  @keyframes showRow {
    from {
      transform: var(--row-translate);
    }
    to {
      transform: translateX(0);
    }
  }

  @keyframes showText {
    from {
      transform: var(--text-translate);
    }
    to {
      transform: translateX(0);
    }
  }

  @keyframes hideRow {
    from {
      transform: translateX(0);
    }
    to {
      transform: var(--row-translate);
    }
  }
</style>
