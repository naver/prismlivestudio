<template>
  <div class="text" :style="{'width':maxWidth}">{{words[wordIndex]}}</div>
</template>

<script>
export default {
  name: 'WordScroll',
  props: {
    words: Array,
    delay: {
      type: Number,
      default: 0
    },
    start: {
      type: Boolean,
      default: false
    },
    offset: {
      type: Number,
      default: 100
    },
    maxWidth: Number
  },
  watch: {
    start (now) {
      now ? this.run() : this.reset()
    }
  },
  data () {
    return {
      wordIndex: 0,
      timer: 0,
      end: 0
    }
  },
  methods: {
    run () {
      setTimeout(() => {
        this.timer = setInterval(() => {
          this.wordIndex = Math.min(this.wordIndex + 1, this.words.length - 1)
          if (this.wordIndex === this.words.length - 1) {
            if (this.end === 0) {
              this.wordIndex = 0
              this.end = 1
            } else {
              clearInterval(this.timer)
            }
          }
        }, this.offset)
      }, this.delay)
    },
    reset () {
      clearInterval(this.timer)
      this.wordIndex = 0
      this.end = 0
    }
  }
}
</script>

<style scoped>
  .text {
    text-align: center;
  }
</style>
