<template>
  <div ref="$box" style="position: relative;">
    <div ref="$content">
      <div class="min-height-row pre-wrap" v-for="(witem,windex) in wordList" :key="'w'+windex">
        <div :class="className" :style="{'background':data.background,...alignStyle}" style="display: flex;position: relative;">
          <div style="text-align: center" :style="{margin:wordMargin}" v-for="(item,index) in witem" :key="index">
            <span v-if="item!==' '">{{item}}</span>
            <span style="display: inline-block;" v-else :style="{'minWidth':wordWidth}"></span>
          </div>
        </div>
      </div>
    </div>
    <div ref="$normal">
      <div class="min-height-row pre-wrap"  v-for="(witem,windex) in wordList" :key="'w'+windex" :style="alignStyle" style="position: absolute;display: flex;visibility: hidden;">
        <div ref="$word" style="text-align: center" :style="{margin:wordMargin}" v-for="(item,index) in witem" :key="index">
          <span v-if="item!==' '">{{item}}</span>
          <span style="display: inline-block;" v-else :style="{'minWidth':wordWidth}"></span>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'Title_B2',
  props: ['data'],
  data () {
    return {
      isShow: false,
      margin: 0,
      timer: 0,
      offset: 0.5,
      maxWordWidth: 0,
      aniClassName: ''
    }
  },
  watch: {
    isShow (now, old) {
      if (now) {
        this.startAddMagin()
      } else {
        clearInterval(this.timer)
      }
    }
  },
  computed: {
    className () {
      return this.isShow ? 'fade-in' : 'fade-out'
    },
    wordList () {
      return this.data.content.map(item => item.split(''))
    },
    wordMargin () {
      return `0px ${this.margin / 50}px`
    },
    wordWidth () {
      return this.maxWordWidth ? (this.maxWordWidth / 2) + 'px' : 'normal'
    },
    alignStyle () {
      return {'justifyContent': ({'left': 'flex-start', 'center': 'center', 'right': 'flex-end'})[this.data.align] || 'center'}
    }
  },
  mounted () {
    this.isShow = true
    let boxW = this.$refs.$box.parentElement.offsetWidth
    let normalWidth = this.$refs.$content.offsetWidth
    if (boxW > normalWidth) {
      let wordW = (boxW - normalWidth) / this.wordList.length
      this.offset = Math.max(Math.min(0.5, (wordW * 60) / 2600), 0.1)
    }
    this.offset = Math.max(this.offset / this.data.animateRate, 0.05)
    this.$refs.$box.addEventListener('animationend', (e) => {
      if (this.aniClassName === e.target.className) {
        return
      }
      this.aniClassName = e.target.className
      if (!this.isShow) {
        this.stopAddMagin()
      } else if (this.data.isOnce) {
        return clearInterval(this.timer)
      }
      setTimeout(() => {
        this.isShow = !this.isShow
      }, 100)
    }, false)
    this.getMaxWordWidth()
  },
  methods: {
    startAddMagin () {
      this.timer = setInterval(() => {
        this.margin += this.offset
      })
    },
    stopAddMagin () {
      clearInterval(this.timer)
      this.margin = 0
    },
    getMaxWordWidth () {
      let list = this.$refs.$word
      let maxWidth = 0
      list.forEach(item => {
        item.offsetWidth > maxWidth && (maxWidth = item.offsetWidth)
      })
      this.maxWordWidth = maxWidth
    }
  }
}
</script>

<style scoped>
  .fade-in {
    animation: fadeIn 2.9s cubic-bezier(0.4, 0, 0.4, 1) both;
  }

  .fade-out {
    animation: fadeOut 0.01s both;
  }

  @keyframes fadeIn {
    0% {
      opacity: 0;
    }
    20% {
      opacity: 1;
    }
    100% {
      opacity: 1;
    }
  }

  @keyframes fadeOut {
    0% {
      opacity: 1;
    }
    100% {
      opacity: 0;
    }
  }
</style>
