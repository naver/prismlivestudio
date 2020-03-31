<template>
  <div style="font-weight: bold">
    <el-autocomplete
      class="inline-input"
      v-model="selected"
      :fetch-suggestions="querySearch"
    ></el-autocomplete>
    <el-button icon="el-icon-plus" size="mini" @click="openAdd"></el-button>
    <el-dialog v-loading="loading" append-to-body :visible.sync="isShowDialog">
      <div class="text">Font Url</div>
      <el-input style="margin-bottom: 20px;" v-model="fontUrl"></el-input>
      <div class="text">Font Name</div>
      <el-input v-model="fontName"></el-input>
      <div style="margin-top: 30px;display: flex;justify-content: flex-end">
        <el-button :disabled="!fontUrl||!fontName" @click="addFont" type="primary">OK</el-button>
      </div>
    </el-dialog>
  </div>
</template>

<script>
import { getFonts } from './util'

export default {
  name: 'FontSelect',
  props: ['value'],
  data () {
    return {
      isShowDialog: false,
      fontUrl: 'https://fonts.googleapis.com/css2?family=Do+Hyeon&display=swap',
      fontName: 'Do Hyeon',
      fontList: [],
      selected: this.value,
      loading: false
    }
  },
  mounted () {
    this.setHistory()
    getFonts().then(res => {
      this.fontList = [...this.fontList, ...res]
    })
  },
  watch: {
    selected (now) {
      this.$emit('input', now)
    },
    value (now) {
      this.selected = now
    }
  },
  methods: {
    openAdd () {
      if (!window.localStorage.fontTemp) {
        this.fontUrl = 'https://fonts.googleapis.com/css2?family=Do+Hyeon&display=swap'
        this.fontName = 'Do Hyeon'
      } else {
        this.fontUrl = this.fontName = ''
      }
      this.isShowDialog = true
    },
    addFont () {
      this.getFontFamliy(this.fontUrl, () => {
        this.$message.success('load font url success')
        this.fontList.unshift(this.fontName)
        this.selected = this.fontName
        this.isShowDialog = false
        this.addHistory(this.fontName, this.fontUrl)
      }, () => {
        this.$message.error('load font url error,please check url')
      })
    },
    querySearch (queryString, cb) {
      cb(this.fontList.map(item => ({value: item})))
    },
    addHistory (key, value) {
      let history = JSON.parse(window.localStorage.fontTemp || '{}')
      history[key] = value
      window.localStorage.fontTemp = JSON.stringify(history)
    },
    setHistory () {
      let history = JSON.parse(window.localStorage.fontTemp || '{}')
      let supported = [...document.fonts.keys()]
      for (let key in history) {
        !supported.find(item => (item.family.indexOf(key) > -1)) ? this.getFontFamliy(history[key], () => {
          this.fontList.unshift(key)
        }) : this.fontList.unshift(key)
      }
    },
    getFontFamliy (url, callback, error) {
      let link = document.createElement('link')
      link.setAttribute('href', url)
      link.setAttribute('rel', 'stylesheet')
      let head = document.getElementsByTagName('head')[0]
      head.appendChild(link)
      this.loading = true
      link.onload = () => {
        // head.removeChild(link)
        this.loading = false
        callback()
      }
      link.onerror = error
    }
  }
}
</script>

<style scoped>
  .text {
    font-weight: bold;
    font-size: 18px;
  }
</style>
