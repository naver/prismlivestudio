<template>
  <div class="main-view-box">
    <div class="main-view" :style="styles">
      <TitleA1 class="animate-item" ref="$target" v-if="data.type==='Title_A1'" :data="data"/>
      <TitleA2 ref="$target" v-else-if="data.type==='Title_A2'" :data="data"/>
      <TitleA3 ref="$target" v-else-if="data.type==='Title_A3'" :data="data"/>
      <TitleA4 ref="$target" v-else-if="data.type==='Title_A4'" :data="data"/>
      <TitleB1 ref="$target" v-else-if="data.type==='Title_B1'" :data="data" :mainStyle="styles"/>
      <TitleB2 ref="$target" v-else-if="data.type==='Title_B2'" :data="data"/>
      <TitleB3 ref="$target" v-else-if="data.type==='Title_B3'" :data="data"/>
      <TitleB4 ref="$target" v-else-if="data.type==='Title_B4'" :data="data"/>
      <Caption4 ref="$target" v-else-if="data.type==='Lower_Third_1'" :data="data"/>
      <Caption5 ref="$target" v-else-if="data.type==='Lower_Third_2'" :data="data"/>
      <Element1 ref="$target" v-else-if="data.type==='Element_1'" v-bind="data"/>
      <Element2 ref="$target" v-else-if="data.type==='Element_2'" v-bind="data"/>
      <Caption1 ref="$target" v-else-if="data.type==='Caption_1'" v-bind="data"/>
      <Caption2 ref="$target" v-else-if="data.type==='Caption_2'" v-bind="data"/>
      <Caption3 ref="$target" v-else-if="data.type==='Caption_3'" :data="data"/>
      <Social ref="$target" v-else-if="(''+data.type).indexOf('Social_')!==-1" :data="data"/>
    </div>
  </div>
</template>

<script>
import TitleA1 from '@/animate/title/TitleA1'
import TitleA2 from '@/animate/title/TitleA2'
import TitleA3 from '@/animate/title/TitleA3'
import TitleA4 from '@/animate/title/TitleA4'
import TitleB1 from '@/animate/title/TitleB1'
import TitleB2 from '@/animate/title/TitleB2'
import TitleB3 from '@/animate/title/TitleB3'
import TitleB4 from '@/animate/title/TitleB4'
import Caption4 from '@/animate/caption/Caption4'
import Caption5 from '@/animate/caption/Caption5'
import Element1 from '@/animate/element/Element1.vue'
import Element2 from '@/animate/element/Element2.vue'
import Caption1 from '@/animate/caption/Caption1.vue'
import Caption2 from '@/animate/caption/Caption2.vue'
import Caption3 from '@/animate/caption/Caption3.vue'
import Social from '@/animate/Social'
import { getObjDataOfKey, getSearch, getFontWeight } from '@/test/util'

const supportType = [
  'Title_A1', 'Title_A2', 'Title_A3', 'Title_A4', 'Title_B1',
  'Title_B2', 'Title_B3', 'Title_B4', 'Element_2', 'Element_1',
  'Caption_3', 'Caption_2', 'Caption_1', 'Lower_Third_1', 'Lower_Third_2',
  'Social_1', 'Social_2', 'Social_3', 'Social_4', 'Social_5', 'Social_6', 'Social_7'
]
export default {
  name: 'MainView',
  components: {TitleA1, TitleA4, TitleA2, TitleA3, TitleB1, TitleB2, TitleB3, TitleB4, Caption4, Social, Element2, Element1, Caption3, Caption2, Caption1, Caption5},
  data () {
    let data = this.getConfig() || {}
    return {
      data: {
        type: supportType.includes(data.type) ? data.type : null,
        timer: 0,
        ...data
      }
    }
  },
  computed: {
    styles () {
      let style = {}
      if (!this.data) return {}
      this.data.fontFamily && (style.fontFamily = this.data.fontFamily)
      this.data.fontSize && (style.fontSize = this.data.fontSize)
      this.data.fontColor && (style.color = this.data.fontColor)
      this.data.textLineSize && this.data.textLineColor && (style.webkitTextStrokeWidth = parseInt(this.data.textLineSize) + 'px')
      this.data.textLineColor && (style.webkitTextStrokeColor = this.data.textLineColor)
      if (this.data.fontStyle) {
        let str = this.data.fontStyle.toLowerCase()
        style.fontWeight = getFontWeight(str)
        ~str.indexOf('italic') && (style.fontStyle = 'italic')
      }
      style.whiteSpace = 'nowrap'
      return style
    }
  },
  mounted () {
    let autoFreshData = localStorage.getItem('autoFreshData')
    if (autoFreshData) {
      localStorage.removeItem('autoFreshData')
      this.data = JSON.parse(autoFreshData)
    }
    this.setAutoRefresh()
  },
  methods: {
    setAutoRefresh () {
      setTimeout(() => {
        localStorage.setItem('autoFreshData', JSON.stringify(this.data))
        location.reload()
      }, 60 * 60 * 1000) // 每个小时刷新一次，预防页面运行太久了出一些怪异性问题
    },
    getConfig () {
      let sData = getSearch()
      return getObjDataOfKey(sData.data ? JSON.parse(sData.data) : {})
    },
    setData (data) {
      this.data.type = ''
      this.processData(data)
      this.$nextTick(() => {
        this.data = data
      })
    },
    reset () {
      this.$refs.$target.reset && this.$refs.$target.reset()
    },
    handleEvent (data) {
      data = data.detail
      if (typeof data === 'string') {
        data = JSON.parse(data)
      }
      // prism编辑时一直发消息，速度太快，延迟处理
      clearTimeout(this.timer)
      this.timer = setTimeout(() => {
        this.setData(data)
      }, 300)
    },
    splitContent (content, splitStr) {
      let list = content.split(splitStr)
      while (!list[list.length - 1] && list.length > 0) {
        list.pop()
      }
      while (!list[0] && list.length > 0) {
        list.shift()
      }
      return list
    },
    processData (data) {
      data.animateRate = data.animateRate ? 1 / data.animateRate : 1

      // lowercase
      let splitStr = '\\n'
      if (data.textTransform === 'lowercase') {
        data.content = data.content.toLowerCase()
        data.subContent = data.subContent.toLowerCase()
      } else if (data.textTransform === 'uppercase') {
        splitStr = '\\N'
        data.content = data.content.toUpperCase()
        data.subContent = data.subContent.toUpperCase()
      }
      data.content = this.splitContent(data.content, splitStr)
      data.subContent = this.splitContent(data.subContent, splitStr)
    }
  },
  created () {
    window.addEventListener('textMotion', this.handleEvent)
  },
  beforeDestroy () {
    window.removeEventListener('textMotion', this.handleEvent)
  }
}
</script>

<style scoped lang="scss">
  .main-view-box {
    color: white;
    height: 100%;
    width: 100%;

    .main-view {
      position: relative;
      box-sizing: border-box;
      height: 100%;
      width: 100%;
      display: flex;
      align-items: center;
      justify-content: center;
      background-size: 100% 100%;
      background-repeat: no-repeat;
      overflow: hidden;
    }
  }
</style>
