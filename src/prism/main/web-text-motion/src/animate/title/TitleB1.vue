<template>
  <div class="container" :style="animateStyle" :class="containerClass" ref="container" @animationend.prevent.stop="animationend">
    <div :style="{'fontSize':data.fontSize||'24px','textAlign':this.data.align||'center'}" class="box wrap-center">
      <div class="min-line" v-for="(witem,windex) in wordList" :key="'w'+windex" :style="{'display':windex===0?'inline-block':'block'}">
        <div v-if="windex===0" style="float: left">{{firstWord}}</div>
        <div class="word" ref="$words"
             :class="wordsClass[getIndex(windex,index)]"
             v-for="(item,index) in witem"
             :key="'w'+index"
             :style="animateStyle">
          <div style="display: inline-block" ref="$words" v-if="item!==' '">{{item}}</div>
          <div style="display: inline-block" v-else :style="{'minWidth':wordWidth}">
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'Title_B1',
  props: ['data'],
  data () {
    let wordList = this.data.content
    let firstWord = wordList[0].substring(0, 1)
    wordList[0] = wordList[0].substring(1)
    let wordLen = 0
    wordList = wordList.map(item => {
      let i = item.split('')
      wordLen += item.length
      return i
    })
    let wordLenOffsetCatch = {}
    wordList.reduce((num, item, index) => {
      wordLenOffsetCatch[index] = num
      num += item.length
      return num
    }, 0)
    return {
      wordList,
      wordLen,
      firstWord,
      containerClass: '',
      wordsClass: [],
      offset: 0,
      wordWidth: '0px',
      wordLenOffsetCatch
    }
  },
  computed: {
    delay () {
      return 1000 / this.wordLen
    },
    animateStyle () {
      let data = {}
      let defaultDuration = (1500 * this.data.animateRate) + 'ms'
      let float = parseFloat(defaultDuration)
      data['--animate-slide-duration'] = float * 0.8 + 'ms'
      data['--animate-up-duration'] = float * 0.2 + 'ms'
      data['--animate-up-delay'] = float * 0.4 + 'ms'
      return data
    }
  },
  mounted () {
    this.getMaxWordWidth()
    this.animationIn()
  },
  methods: {
    animationIn () {
      this.containerClass = 'container-in'
      setTimeout(this.wordIn)
    },
    animationOut () {
      this.containerClass = 'container-out'
    },
    getIndex (windex, index) {
      return this.wordLenOffsetCatch[windex] + index
    },
    wordIn () {
      this.wordsClass.push('word-in')
      setTimeout(() => {
        this.wordLen > this.wordsClass.length && this.wordIn()
      }, this.offset)
    },
    wordOut () {
      this.reset()
      this.$nextTick(() => this.animationIn())
    },
    getOffset () {
      this.offset = 250 / this.wordLen
    },
    getMaxWordWidth () {
      let list = this.$refs.$words
      if (!list) return
      let maxWidth = 10
      list.forEach(item => {
        item.offsetWidth > maxWidth && (maxWidth = item.offsetWidth)
      })
      this.wordWidth = maxWidth / 2 + 'px'
    },
    reset () {
      this.containerClass = ''
      this.wordsClass = []
    },
    animationend (el) {
      if (el.target !== this.$refs.container) return false
      let isOut = el.target.classList.contains('container-out')
      if (isOut) {
        this.reset()
        this.$nextTick(this.animationIn)
      } else if (!this.data.isOnce) {
        setTimeout(() => {
          this.animationOut()
        }, 1000)
      }
    }
  }
}
</script>

<style scoped lang="scss">
  .container {
    opacity: 0;
    --animate-slide-duration: .8s;
    --animate-up-duration: .2s;
    animation-fill-mode: both;

    &.container-in {
      animation-name: containerIn;
      animation-timing-function: cubic-bezier(0, 0, 0.1, 1);
      animation-duration: var(--animate-slide-duration);
    }

    &.container-out {
      animation-name: fadeOut;
      animation-duration: var(--animate-up-duration);
    }

    .box {
      width: 100%;
      text-align: center;
      font-size: 0;

      div {
        display: inline-block;
      }
    }

    .word {
      transform: translate3d(0, 100%, 0);
      opacity: 0;
      --animate-up-duration: .2s;
      --animate-up-delay: .4s;
      animation-fill-mode: both;
      animation-duration: var(--animate-up-duration);
      animation-delay: var(--animate-up-delay);
      animation-timing-function: cubic-bezier(0.6, 0, 0, 1);

      &.word-in {
        animation-name: wordIn;
        transform: translate3d(0, 100%, 0);
      }
    }
  }

  @keyframes containerIn {
    0% {
      transform: translate3d(50%, 50%, 0);
      opacity: 0;
    }
    50% {
      opacity: 1;
      transform: translate3d(50%, 0, 0);
    }
    100% {
      transform: translate3d(0, 0, 0);
      opacity: 1;
    }
  }

  @keyframes containerOut {
    0% {
      transform: translate3d(50%, 50%, 0);
      opacity: 0;
    }
    50% {
      opacity: 1;
      transform: translate3d(50%, 0, 0);
    }
    100% {
      transform: translate3d(0, 0, 0);
      opacity: 1;
    }
  }

  @keyframes wordIn {
    0% {
      opacity: 0;
      transform: translate3d(0, 100%, 0);
    }
    100% {
      opacity: 1;
      transform: translate3d(0, 0, 0);
    }
  }
</style>
