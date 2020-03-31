export default {
  props: ['data'],
  computed: {
    style () {
      let style = {}
      if (!this.data) return {}
      this.data.fontFamily && (style.fontFamily = this.data.fontFamily)
      this.data.fontSize && (style.fontSize = this.data.fontSize)
      this.data.fontColor && (style.color = this.data.fontColor)
      this.data.fontWeight && (style.fontWeight = this.data.fontWeight)
      style.whiteSpace = 'nowrap'
      return style
    }
  }
}
