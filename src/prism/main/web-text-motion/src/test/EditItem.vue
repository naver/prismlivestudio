<template>
  <div>
    <div style="display: flex;">
      <div style="margin-right: 20px;">
        <div style="margin-bottom: 10px;">
          <div class="explain" v-if="type==='Caption_2'||type==='Title_A4'||type==='Caption_1'||type==='Caption_3'">
            explain: \n used to line breaks
          </div>
          <div class="explain" v-if="type==='Caption_2'">
            explain: \l used to highlight line breaks
          </div>
          <div class="explain" v-if="type==='Title_B3'">
            explain: Group using ',' for example, input 'ABCDE,12345' , show 'A' to 'E' and '1' to '5',Finally shown as 'E5'
          </div>
        </div>
        <div class="input-item">
          <div class="label">Content</div>
          <div class="value">
            <textarea style="font-size: 16px;" type="text" rows="5" cols="100" v-model="content"></textarea>
          </div>
        </div>
        <div class="input-item">
          <div class="label">Sub Content</div>
          <div class="value">
            <el-input v-model="subContent"></el-input>
          </div>
        </div>
        <div class="input-item">
          <div class="label">Text Transform</div>
          <div class="value">
            <el-input v-model="textTransform"></el-input>
          </div>
        </div>
      </div>
      <div>
        <div class="input-item">
          <div class="label">Font Size</div>
          <div class="value">
            <el-input v-model="fontSize"></el-input>
          </div>
        </div>
        <div class="input-item">
          <div class="label">Font Color</div>
          <div class="value">
            <el-color-picker v-model="fontColor" show-alpha></el-color-picker>
          </div>
        </div>
        <div class="input-item">
          <div class="label">Font Style</div>
          <div class="value">
            <el-input v-model="fontStyle"></el-input>
          </div>
        </div>
        <div class="input-item">
          <div class="label">Background</div>
          <div class="value">
            <el-color-picker v-model="background" show-alpha></el-color-picker>
          </div>
        </div>
        <div class="input-item">
          <div class="label">Font Name</div>
          <div class="value">
            <FontSelect v-model="fontFamily"></FontSelect>
          </div>
        </div>
      </div>
      <div style="margin-left: 20px;">
        <div class="input-item">
          <div class="label">Once Animate</div>
          <div class="value">
            <el-switch
              v-model="isOnce">
            </el-switch>
          </div>
        </div>
        <div class="input-item">
          <div class="label">Stroke Color</div>
          <div class="value">
            <el-color-picker v-model="textLineColor" show-alpha></el-color-picker>
          </div>
        </div>
        <div class="input-item">
          <div class="label">Stroke Size</div>
          <div class="value">
            <el-input v-model="textLineSize"></el-input>
          </div>
        </div>
        <div class="input-item">
          <div class="label">Align</div>
          <div class="value">
            <el-select v-model="align">
              <el-option
                v-for="item in ['left','center','right']"
                :key="item"
                :label="item"
                :value="item">
                {{item}}
              </el-option>
            </el-select>
          </div>
        </div>
        <div class="input-item">
          <div class="label">Animate Rate</div>
          <div class="value">
            <el-input v-model="animateRate"></el-input>
          </div>
        </div>
      </div>
    </div>
    <div style="display: flex;justify-content: flex-end">
      <el-button type="primary" @click="save">OK</el-button>
    </div>
  </div>
</template>

<script>
import FontSelect from './FontSelect'
import BgSelected from './BgSelected2'
import { getObjDataOfKey } from '@/test/util'

const temp = {
  content: '',
  upText: '',
  downText: '',
  subContent: '',
  align: 'left',
  background: '',
  fontColor: '',
  fontFamily: '',
  fontSize: '20pt',
  fontStyle: 'Normal',
  textTransform: 'none',
  type: '',
  isOnce: false,
  animateRate: '1',
  textLineColor: '',
  textLineSize: '0'
}
export default {
  name: 'EditItem',
  components: {BgSelected, FontSelect},
  data () {
    return {
      backgrounList: [
        {
          type: 'color',
          value: 'rgba(38, 38, 38, 1)'
        }
      ],
      originData: null,
      ...JSON.parse(JSON.stringify(temp))
    }
  },
  computed: {
    hasCubicBezier () {
      return ['Title_A1', 'Title_A2', 'Title_A3', 'Title_A4', 'Title_B1', 'Title_B3'].includes(this.type)
    },
    hasCubicBezier2 () {
      return ['Title_A3', 'Title_B1'].includes(this.type)
    }
  },
  methods: {
    save () {
      let data = getObjDataOfKey(this)
      data.fontSize = this.fontSize
      this.$emit('saveData', {...this.originData, ...data, backgrounList: this.backgrounList})
    },
    reset () {
      let target = JSON.parse(JSON.stringify(temp))
      for (let key in target) {
        this[key] = target[key]
      }
    },
    setData (data) {
      this.reset()
      this.originData = JSON.parse(JSON.stringify(data))
      let target = getObjDataOfKey(JSON.parse(JSON.stringify(data)))
      for (let key in target) {
        this[key] = target[key]
      }
      console.log('updateEdit', data)
    }
  }
}
</script>

<style scoped>
  .input-item {
    margin-bottom: 20px;
  }

  .label {
    color: black;
    font-weight: bold;
    font-size: 18px;
  }

  .explain {
    font-size: 18px;
    font-weight: bold;
  }
</style>
