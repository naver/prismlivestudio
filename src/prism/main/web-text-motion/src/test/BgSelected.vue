<template>
  <div style="font-weight: bold;display: flex;align-items: center;">
    <el-color-picker v-model="color" show-alpha></el-color-picker>
    <el-button @click="selectPicture" slot="reference" circle size="mini" style="margin-left: 15px;font-size: 18px;" icon="el-icon-picture"></el-button>
    <input style="display: none" ref="$file" type="file" @change="fileChange" accept="image/*">
  </div>
</template>

<script>

export default {
  name: 'BgSelect',
  props: ['value'],
  data () {
    return {
      type: 'color',
      color: 'rgba(38, 38, 38, 1)',
      url: ''
    }
  },
  watch: {
    color (now) {
      if (now) {
        this.type = 'color'
        this.$emit('input', {
          type: 'color',
          value: now
        })
      }
    },
    url (now) {
      if (now) {
        this.type = 'color'
        this.$emit('input', {
          type: 'image',
          value: this.url
        })
      }
    },
    value: {
      handler (now) {
        this.type = now.type
        this.type === 'color' ? (this.color = now.value) : (this.url = now.value)
      },
      deep: true
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
