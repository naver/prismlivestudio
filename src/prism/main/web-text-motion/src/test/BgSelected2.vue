<template>
  <div style="font-weight: bold;display: flex;align-items: center;">
    <div class="show-box">
      <div class="show-item" v-for="(item,index) in bgList" :key="index">
        <i v-if="bgList.length>1" @click="removeBg(index)" class="el-icon-close" style="position: absolute;
    right: -15px;
    top: -15px;
    cursor: pointer;
    font-size: 18px;"></i>
        <div
          style="width: 100%;height: 100%;"
          :style="{'backgroundColor':item.type==='color'?item.value:'','backgroundImage':item.type==='image'?`url(${item.value})`:''}">
        </div>
      </div>
    </div>
    <div style="display: flex;align-items: center">
      <el-color-picker v-model="color" show-alpha></el-color-picker>
      <el-button @click="selectPicture" slot="reference" circle size="mini" style="margin-left: 15px;font-size: 18px;" icon="el-icon-picture"></el-button>
      <input style="display: none" ref="$file" type="file" @change="fileChange" accept="image/*">
    </div>
  </div>
</template>

<script>

export default {
  name: 'BgSelect',
  props: ['value'],
  data () {
    return {
      color: 'rgba(38, 38, 38, 1)',
      url: '',
      bgList: this.value
    }
  },
  watch: {
    bgList: {
      handler (now) {
        this.$emit('input', now)
      },
      deep: true
    },
    color (now) {
      if (now) {
        this.bgList.push({
          type: 'color',
          value: now
        })
      }
    },
    url (now) {
      if (now) {
        this.bgList.push({
          type: 'image',
          value: this.url
        })
      }
    }
  },
  methods: {
    selectPicture () {
      this.$refs.$file.click()
    },
    fileChange (tar) {
      let file = tar.target.files[0]
      let reader = new FileReader()
      reader.readAsDataURL(file)
      reader.onload = (res) => {
        this.url = res.target.result
        this.$refs.$file.value = ''
      }
    },
    removeBg (index) {
      this.bgList.splice(index, 1)
    },
    setValue (value) {
      this.bgList = value || []
    }
  }
}
</script>

<style scoped>
  .text {
    font-weight: bold;
    font-size: 18px;
  }

  .show-box {
    display: flex;
  }

  .show-item {
    position: relative;
    width: 200px;
    height: 200px;
    margin-right: 40px;
    background-size: 100% 100%;
    background-repeat: no-repeat;
  }
</style>
