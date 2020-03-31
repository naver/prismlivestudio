<template>
  <div class="box">
    <div class="item">
      <div class="des">R</div>
      <input type="number" v-model="data.R">
    </div>
    <div class="item">
      <div class="des">G</div>
      <input type="number" v-model="data.G">
    </div>
    <div class="item">
      <div class="des">B</div>
      <input type="number" v-model="data.B">
    </div>
    <div class="item">
      <div class="des">Opicity</div>
      <input type="number" v-model="data.A">
    </div>
  </div>
</template>

<script>
export default {
  name: 'ColorSelect',
  data () {
    return {
      data: {
        A: 1,
        R: 255,
        G: 255,
        B: 255
      }
    }
  },
  watch: {
    data: {
      handler (v, l) {
        this.$emit('update:color', `rgba(${this.data.R},${this.data.G},${this.data.B},${this.data.A || 1})`)
      },
      deep: true
    }
  }
}
</script>

<style scoped lang="scss">
  .box {
    display: flex;

    .item {
      margin: 5px;
      display: flex;

      .des {
        margin-right: 4px;
      }

      input {
        width: 40px;
      }
    }
  }
</style>
