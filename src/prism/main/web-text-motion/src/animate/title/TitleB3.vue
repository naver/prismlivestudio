<template>
  <div style="padding:25px;box-sizing: border-box">
    <div ref="el" :class="classNames" class="container">
      <div>
        <div class="min-height-row pre-wrap" :style="alignStyle" v-for="(witem,windex) in wrapWordList" :key="'w'+windex">
          <template v-for="(item,index) in witem">
            <div :key="index">
              <WordScroll
                :key="index"
                v-if="item.list.length!==0"
                :style="{'width':maxWordWidth+'px'}"
                :start="item.index<showIndex"
                :class="{'animate visible':item.index<showIndex}"
                :words="item.list"
                :delay="item.index*offset"
                :offset="offset"
                class="words"/>
              <div v-else>&nbsp;</div>
            </div>
          </template>
        </div>
      </div>
      <div v-if="data.subContent"
           :class="{'slide-in':isShowSubTitle,'visible':isShowSubTitle}"
           :style="{...alignStyle,'--animate-duration':800*data.animateRate+'ms'}"
           class="sub-title-box animate">
        <div :style="subStyle" :class="{'animate-fade-in':isShowSubTitle}" class="sub-title animate">
          <div :style="{'text-align':data.align}" class="wrap-style min-height-row pre-wrap" v-for="(item,index) in data.subContent" :key="index">{{item}}</div>
        </div>
      </div>
      <div style="visibility: hidden;position: absolute;">
      <span ref="$word" v-for="(item,index) in tempList" :key="index">{{item}}</span>
      </div>
    </div>
  </div>
</template>

<script>
import WordScroll from '@/components/WordScroll'

export default {
  name: 'Title_B3',
  components: {WordScroll},
  props: ['data'],
  data () {
    return {
      isShow: false,
      isShowSubTitle: false,
      showIndex: -1,
      offset: 80 * this.data.animateRate,
      tempList: null,
      maxWordWidth: 0
    }
  },
  watch: {
    wordList: {
      handler (now) {
        this.tempList = now.join('').split('')
        this.$nextTick(() => {
          this.getMaxWordWidth()
        })
      },
      immediate: true
    }
  },
  computed: {
    transLen () {
      let list = this.wordContent.split('').filter(item => (item !== ' '))
      return list.slice(0, Math.min(5, list.length)).length || 1
    },
    alignStyle () {
      return {
        justifyContent: ({left: 'flex-start', center: 'center', 'right': 'flex-end'})[this.data.align],
        display: 'flex',
        flexWrap: 'wrap'
      }
    },
    isShowSubLimit () {
      let last = Math.ceil(((2000 + this.transLen * this.offset) / this.offset) / this.transLen)
      return Math.max(0, this.wordList.length - last)
    },
    classNames () {
      return this.isShow ? 'animate' : 'animate animate-fade-out'
    },
    wrapIndex () {
      if (this.data.content.length < 2) {
        return []
      }
      let result = []
      let index = -1
      this.data.content.forEach(item => {
        index = index + item.length
        result.push(index)
      })
      result.pop()
      return result
    },
    wrapWordList () {
      if (!this.wrapIndex.length) {
        return [this.wordList]
      }
      let result = []
      let now = 0
      let container = []
      let addRow = () => {
        result[now] = container
        container = []
        now++
      }
      let wrap = this.wrapIndex.slice()
      this.wordList.forEach((item, index) => {
        container.push(item)
        if (index !== this.wordList.length && wrap.includes(index)) {
          while (wrap.includes(index)) {
            addRow()
            wrap.splice(wrap.findIndex(j => (j === index)), 1)
          }
        } else {
          container.length && (result[now] = container)
        }
      })
      return result
    },
    wordContent () {
      return this.data.content.reduce((str, item) => {
        str += item
        return str
      }, '')
    },
    wordList () {
      let list = this.wordContent.split('')
      let transList = list.filter(item => (item !== ' ')).slice(0, Math.min(5, list.length))
      let result = []
      list.forEach((item, id) => {
        if (item === ' ') {
          result[id] = []
        } else {
          let li = transList.slice().sort(() => (0.5 - Math.random()))
          li.push(item)
          result[id] = li
        }
      })
      result = result.map((item, index) => {
        return {
          list: item,
          index
        }
      })
      return result
    },
    subStyle () {
      let size = this.data.fontSize || ''
      if (!size) return {}
      let num = parseInt(size)
      let unit = size.substring((num + '').length)
      return {
        fontSize: (num * 3 / 10) + unit,
        background: this.data.background || 'white'
      }
    }
  },
  mounted () {
    this.runAnimate()
    this.$refs.el.addEventListener('animationend', () => {
      if (!this.isShow) {
        this.isShowSubTitle = false
        this.runAnimate()
      }
    }, false)
  },
  methods: {
    // 所有文字中，最宽的那一个字的宽度作为所有文字的宽度
    getMaxWordWidth () {
      let list = this.$refs.$word
      let maxWidth = 0
      list.forEach(item => {
        item.offsetWidth > maxWidth && (maxWidth = item.offsetWidth)
      })
      this.maxWordWidth = maxWidth
    },
    runAnimate () {
      this.showIndex = -1
      this.isShow = true
      this.timer = setInterval(() => {
        if (this.showIndex > this.wordList.length) {
          clearInterval(this.timer)
          !this.data.isOnce && setTimeout(() => {
            this.isShow = false
          }, (this.wordList.length * 80 + 2000) * this.data.animateRate)
        } else {
          ++this.showIndex
          if (this.showIndex > this.isShowSubLimit) {
            this.isShowSubTitle = true
          }
        }
      }, 100)
    }
  }
}
</script>

<style scoped lang="scss">
  .animate {
    animation-duration: 1s;
    animation-fill-mode: both;
  }

  .container {
    width: 100%;
  }

  .sub-title-box {
    font-size: 8pt;
    margin-top: 10px;
    visibility: hidden;
    display: flex;

    &.visible {
      visibility: visible;
    }
  }

  .sub-title {
    padding: 2px 10px;
    font-size: 12px;
    color: black;
    display: inline;
    letter-spacing: 6px;
  }

  .words {
    margin: 0 4px;
    visibility: hidden;

    &.visible {
      visibility: visible;
      animation-name: fadeIn;
    }
  }

  .slide-in {
    animation-name: slideIn;
    animation-duration: var(--animate-duration);
    animation-delay: 200ms;
    animation-timing-function: cubic-bezier(0.4, 0, 0.4, 1);
  }

  @keyframes slideIn {
    from {
      transform: translate3d(0, 50%, 0);
      visibility: hidden;
    }

    to {
      transform: translate3d(0, 0, 0);
      visibility: visible;
    }
  }

</style>
