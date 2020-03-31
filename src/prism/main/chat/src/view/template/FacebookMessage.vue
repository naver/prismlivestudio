<template>
  <div class="message">
    <div v-if="data.message">{{data.message}}</div>
    <div v-else-if="attachment">
      <div style="width: 80px;height: 80px;" v-if="attachment.type==='sticker'">
        <img style="width: 100%;height: 100%" :src="attachment.url" :alt="attachment.type">
      </div>
      <div style="width: 80px;height: 80px;" v-else-if="attachment.type==='animated_image_share'">
        <video v-if="attachment.media.source" loop style="width: 100%;height: 100%" :src="attachment.media.source" autoplay></video>
        <img style="width: 100%;height: 100%" v-else :src="attachment.media.image.url">
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'template-facebook-chat',
  props: {
    data: {
      type: Object,
      default () {
        return {}
      }
    }
  },
  data () {
    return {}
  },
  computed: {
    attachment () {
      return this.data.rawMessage && this.data.rawMessage.attachment
    },
    isImage () {
      if (!this.attachment) return false
      return this.attachment.type === 'animated_image_share' || this.attachment.type === 'sticker'
    }
  }
}
</script>

<style lang="scss">
</style>
