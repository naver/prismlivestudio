<template>
  <div>
    <div style="font-weight: bold;display: flex;justify-content: space-between;align-items: center">
      <div>{{type.toUpperCase()}}</div>
      <el-popover
        v-model="visible"
        placement="bottom"
        trigger="manual">
        <i class="el-icon-setting" slot="reference" @click="openEdit" style="cursor: pointer;"></i>
      </el-popover>
    </div>
    <MainView class="view-item" ref="$actual"></MainView>
  </div>
</template>

<script>
import MainView from '../components/MainView'

export default {
  name: 'ShowItem',
  components: {MainView},
  props: ['showData'],
  watch: {
    showData: {
      handler (now) {
        this.data = this.showData
        this.setData(now)
        this.editData = now
      },
      deep: true
    }
  },
  data () {
    return {
      visible: false,
      type: '',
      data: {},
      editData: {}
    }
  },
  methods: {
    openEdit () {
      !this.visible && this.$refs.$edit.setData(this.data)
      this.visible = !this.visible
    },
    setData () {
      let data = this.data
      this.type = data.type
      this.$refs.$actual.setData(data)
    },
    hide () {
      this.visible = false
    },
    updateData (data) {
      this.data = data
      this.setData(data)
    }
  },
  mounted () {
    this.data = JSON.parse(JSON.stringify(this.showData))
    this.setData()
  }
}
</script>

<style scoped>
  .view-item {
    height: 100%;
    width: 100%;
  }
</style>
