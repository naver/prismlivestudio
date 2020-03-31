<template>
  <div>
    <div :style="{...alignFlexStyle,'minWidth':lightBoxWidth}" class="row-box" v-for="(item, index) in contentArr" :key="index">
      <div v-if="index!==contentArr.length-1" ref="$items" class="text-box" :class="classArr[index]" :style="animateOptions">
        <div class="text">
          <div class="min-height-row wrap-style">{{item}}</div>
        </div>
      </div>
      <div v-else style="width: 100%;display: flex;" :style="alignFlexStyle">
        <div ref="$items" class="light-box" :class="classArr[index]" :style="{...animateOptions,...lightStyle,'height':lightBoxHeight}">
          <div style="position: absolute;top: 0;left: 0;" :style="{'width':parseInt(lightBoxWidth)+'px'}">
            <div style="padding: 0 5px;">
              <div class="min-height-row wrap-style" v-for="(item1,index1) in item" :key="index1">{{item1}}</div>
            </div>
          </div>
        </div>
        <div ref="$temp" style="position: absolute;visibility:hidden;display: inline-block;">
          <div>
            <div class="min-height-row wrap-style" v-for="(item1,index1) in item" :key="index1">{{item1}}</div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'Caption_2',
  props: {
    content: Array,
    subContent: Array,
    background: String,
    align: String,
    animateRate: [String, Number],
    textTransform: String,
    isOnce: null
  },
  data () {
    return {
      classArr: [],
      showCount: -1,
      lightBoxWidth: null,
      lightBoxHeight: null
    }
  },
  watch: {
    showCount (now, old) {
      if (now === -1) {
        this.classArr = new Array(this.contentArr.length).fill('hide')
      } else if (now === 0) {
        this.classArr = []
      } else if (now > 0) {
        this.classArr = new Array(now).fill('show')
      }
    }
  },
  computed: {
    animateOptions () {
      return {
        '--animation-duration': this.animateDuration,
        '--animation-pos': this.align === 'right' ? '50px' : '-50px',
        '--animation-width': this.lightBoxWidth,
        'textAlign': this.align
      }
    },
    alignFlexStyle () {
      return {'justifyContent': ({'left': 'flex-start', 'center': 'center', 'right': 'flex-end'})[this.align] || 'center'}
    },
    animateDuration () {
      return 450 * this.animateRate + 'ms'
    },
    lightStyle () {
      return {
        background: this.background
      }
    },
    contentArr () {
      return [...this.content, this.subContent]
    }
  },
  methods: {
    ready () {
      let items = this.$refs.$items || []
      items.forEach((el, index) => {
        el.addEventListener('animationend', () => {
          if (this.showCount === this.contentArr.length) {
            !this.isOnce && setTimeout(() => (this.showCount = -1), 2000)
          }
          if (this.showCount > 0 && index < this.contentArr.length) {
            this.showCount++
          } else if (this.showCount === -1) {
            setTimeout(() => (this.showCount = 1), 500)
          }
        })
      })
    },
    reset () {
      this.showCount = 0
    }
  },
  mounted () {
    this.ready()
    this.lightBoxWidth = (this.$refs.$temp[0].offsetWidth + 11) + 'px'
    this.lightBoxHeight = this.$refs.$temp[0].offsetHeight + 'px'
    this.showCount = 1
  }
}
</script>

<style scoped lang="scss">
  $ani-time: .4s;
  .text {
    display: inline-block;
  }

  .row-box {
    overflow: hidden;
    margin: 8px;
    display: flex;
  }

  .text-content {
    display: inline-block;
  }

  .text-box {
    --animation-duration: 400ms;
    --animation-pos: 50px;
    opacity: 0;
    transform: translateX(-50px);
    animation-duration: var(--animation-duration);
    animation-fill-mode: both;
    animation-timing-function: ease;

    &.show {
      animation-name: defaultShow;
    }

    &.hide {
      animation-name: defaultHide;
    }
  }

  .light-box {
    box-sizing: border-box;
    position: relative;
    overflow: hidden;
    width: 0;
    --animation-width: 500px;
    --animation-duration: 400ms;

    &.show {
      animation: lightShow var(--animation-duration) cubic-bezier(0.33, 0, .4, 1) both;
    }

    &.hide {
      animation: lightHide var(--animation-duration) cubic-bezier(0.33, 0, .4, 1) both;
    }
  }

  @keyframes defaultShow {
    from {
      opacity: 0;
      transform: translateX(var(--animation-pos));
    }
    to {
      opacity: 1;
      transform: translateX(0);
    }
  }

  @keyframes defaultHide {
    from {
      opacity: 1;
      transform: translateX(0);
    }
    to {
      opacity: 0;
      transform: translateX(var(--animation-pos));
    }
  }

  @keyframes lightShow {
    from {
      width: 0;
    }
    to {
      width: var(--animation-width);
    }

  }

  @keyframes lightHide {
    from {
      width: var(--animation-width);
    }
    to {
      width: 0;
    }

  }
</style>
