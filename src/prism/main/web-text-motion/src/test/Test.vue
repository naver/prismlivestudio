<template>
  <div class="box">
    <div class="input-box border-bottom" style="font-size: 24px;font-weight:bold;">
      PRISM Text Motion Template Test Web
    </div>
    <div class="border-bottom" style="padding-bottom: 0">
      <div style="display: flex;align-items: center;justify-content: space-between">
        <div style="font-weight: bold;margin-top: 10px;font-size: 18px">Template Select</div>
        <div style="display: flex;align-items: center;margin-top: 15px;">
          <div style="margin-right: 10px;font-size: 16px;font-weight: bold;">group:</div>
          <el-select v-model="groupSelected">
            <el-option
              v-for="item in groups"
              :key="item"
              :label="item"
              :value="item">
            </el-option>
          </el-select>
        </div>
      </div>
      <div v-if="isShowGif" class="temp-select-area" style="padding: 20px 0;display: flex;flex-wrap: wrap">
        <div @click="tempSelected = index" :class="{'temp-focus':index===tempSelected}" :key="index" v-for="(value,key,index) in gifs" class="gif-item">
          <div style="margin-bottom: 5px;">{{key}}</div>
          <img draggable="false" :src="value" width="150" height="150">
        </div>
      </div>
    </div>
    <div class="border-bottom">
      <div style="font-weight: bold;margin-top: 10px;font-size: 24px">Text Edit</div>
      <div>
        <EditItem @saveData="editUpdateData" ref="$edit"></EditItem>
      </div>
    </div>
    <el-dialog
      append-to-body
      width="1000px"
      top="2vh"
      :before-close="beforeClose"
      :visible.sync="drawer">
      <div>
        <div class="input-box border-bottom" style="font-size: 24px;font-weight:bold;width: 100%;">
          PRISM Text Motion Template Test Web
        </div>
        <div>
          <el-button @click="changeSize">ChangeSize</el-button>
        </div>
        <div style="min-height: 500px;background: gray;">
          <div class="show-box">
            <div :style="{'width':size+'px',height:size+'px'}" class="show-item-pre" v-for="(item,index) in dataList" :key="index">
              <MainView class="view-item" ref="$actual"></MainView>
            </div>
          </div>
        </div>
      </div>
    </el-dialog>
  </div>
</template>

<script>

import EditItem from './EditItem'
import MainView from '../components/MainView'
import FontSelect from './FontSelect'
import BgSelected from './BgSelected'
import { TEST_COMP, TEST_GIF, LANGS, getGroup } from './util'

export default {
  name: 'test3',
  components: {EditItem, FontSelect, BgSelected, MainView},
  data () {
    return {
      drawer: false,
      weights: ['100', '200', '300', '400', '500', '600', '700', '800', '900', '1000'],
      tempSelected: 0,
      langSelected: 'english',
      LANGS,
      TEST_COMP,
      TEST_GIF,
      dataList: [],
      size: 400,
      groupSelected: 'title',
      isShowGif: true,
      groups: ['title', 'caption', 'social', 'element']
    }
  },
  watch: {
    typeSelected (now) {
      now && this.updateEdit()
    },
    groupSelected () {
      this.tempSelected = 0
    }
  },
  mounted () {
    this.updateEdit()
  },
  computed: {
    gifShow () {
      return TEST_GIF[this.langSelected]
    },
    typeSelected () {
      return this.showTypes[this.tempSelected]
    },
    gifs () {
      let map = this.gifShow
      let group = this.groupSelected
      let result = {}
      for (let key in map) {
        getGroup(key) === group && (result[key] = map[key])
      }
      return result
    },
    showTypes () {
      let map = this.gifShow
      let group = this.groupSelected
      let result = []
      for (let key in map) {
        getGroup(key) === group && result.push(key)
      }
      return result
    },
    tempData () {
      let temp = JSON.parse(JSON.stringify(this.TEST_COMP[this.langSelected]))
      let item = temp[this.typeSelected]
      let tar = item.data
      return {
        ...tar,
        type: this.typeSelected,
        fontColor: tar.fontColor || 'rgba(255,255,255,1)',
        fontWeight: tar.fontWeight || 'normal',
        fontFamily: tar.fontFamily || '',
        fontFamilyUrl: tar.fontFamilyUrl || ''
      }
    }
  },
  methods: {
    changeSize () {
      this.size = this.size === 400 ? 800 : 400
    },
    editUpdateData (data) {
      this.drawer = true
      let list = []
      data.backgrounList.forEach(item => {
        let source = JSON.parse(JSON.stringify(data))
        delete source.backgroundList
        source[item.type === 'color' ? 'backgroundColor' : 'backgroundImage'] = item.value
        list.push(source)
      })
      this.dataList = list
      this.$nextTick(() => {
        this.$refs.$actual.forEach((item, index) => item.setData(list[index]))
      })
    },
    beforeClose (next) {
      this.dataList = []
      next()
    },
    updateEdit () {
      this.$refs.$edit.setData(this.tempData)
    }
  }
}
</script>

<style scoped lang="scss">
  .border-bottom {
    border-bottom: 1px solid #dcdfe6;
    padding-bottom: 20px;
  }

  .gif-item {
    margin-right: 10px;
    margin-bottom: 10px;
    width: 185px;
    cursor: pointer;
    display: flex;
    justify-content: center;
    align-items: center;
    flex-direction: column;
    padding: 10px 0;
    border-radius: 10px;

    &:hover, &.temp-focus {
      background: rgba(72, 208, 250, 0.67);
    }
  }

  .box {
    padding: 20px 50px;

    .input-item {
      margin-left: 20px;
      display: flex;
      align-items: center;

      > * {
        margin-right: 20px;
      }
    }
  }

  .show-box {
    margin: 20px 0;
    display: flex;
    flex-wrap: wrap;

    .show-item {
      padding: 20px;
    }
  }

  .test-item {
    height: 400px;
    width: 400px;
  }

  .show-item-pre {
    margin-right: 20px;
    margin-bottom: 20px;
    display: flex;
    align-items: center;
    justify-content: center;
  }
</style>
<style>
  #app {
    /*overflow: scroll !important;*/
  }
</style>
