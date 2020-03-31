<template>
  <span class="content" v-html="message"></span>
</template>

<script>
import {
  YOUTUBE_EMOTICON_REGEX,
  YOUTUBE_EMOTICON
} from '@/assets/js/youtubeResources.js'

export default {
  name: 'template-youtube-message',
  props: {
    data: {
      type: Object,
      default () {
        return {}
      }
    }
  },
  computed: {
    message () {
      let message = this.data.message
      message = message.replace(YOUTUBE_EMOTICON_REGEX, (match, p) => {
        return this.handlePicture(match, p)
      })

      return message
    }
  },
  methods: {
    handlePicture (match, p) {
      const src = YOUTUBE_EMOTICON[match]
      if (src) {
        return `<img class="afreecatv-emoticon" src="${src}" alt="${p}"/>`
      }
    }
  }
}
</script>
