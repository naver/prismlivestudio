<template>
  <div class="box">
    <div style="overflow: hidden;" :style="{height:contentHeight,'text-align':data.align}">
      <div ref="$content" class="title-box" :class="{'is-once':data.isOnce}" :style="style">
        <div
          :style="{...alignStyle,...durationStyle}"
          class="title-text"
          :class="{'is-once':data.isOnce}">
          <div class="min-height-row pre-wrap" v-for="(item,index) in data.content" :key="index">{{item}}</div>
        </div>
      </div>
    </div>
    <div v-if="data.subContent" style="overflow: hidden;" :style="{'text-align':data.align}">
      <div class="sub-title-box" :class="{'is-once':data.isOnce}" :style="subStyle">
        <div :style="alignStyle" class="sub-title-text">
          <div class="min-height-row pre-wrap wrap-style" v-for="(item,index) in data.subContent" :key="index">{{item}}</div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'LowerThird2',
  props: ['data'],
  data () {
    return {
      contentHeight: 'auto'
    }
  },
  computed: {
    style () {
      return {
        background: this.data.background || 'white',
        color: this.data.fontColor || 'black',
        ...this.durationStyle,
        ...this.alignStyle
      }
    },
    subStyle () {
      let size = this.data.fontSize || ''
      if (!size) return {}
      let num = parseInt(size)
      let unit = size.substring((num + '').length)
      return {
        fontSize: (num * 0.6) + unit,
        color: this.style.background,
        background: this.style.color,
        ...this.durationStyle
      }
    },
    durationStyle () {
      let defaultDuration = 3900
      return {
        '--animation-duration': (defaultDuration * this.data.animateRate) + 'ms'
      }
    },
    alignStyle () {
      return {
        'textAlign': this.data.align,
        '--translate-value': this.data.align === 'right' ? 'translateX(110%)' : 'translateX(-110%)'
      }
    }
  },
  mounted () {
    this.contentHeight = this.$refs.$content.offsetHeight - 1 + 'px'
  }
}
</script>

<style scoped lang="scss">
  .box {
    text-align: left;
    box-sizing: border-box;
  }

  .title-box {
    overflow: hidden;
    transform: var(--translate-value);
    display: inline-block;
    padding: 3px 7px;
    animation: titleBoxShow var(--animation-duration) infinite cubic-bezier(0.3, 0, 0.3, 1);

    &.is-once {
      animation: titleBoxShowOnce var(--animation-duration) cubic-bezier(0.3, 0, 0.3, 1) both;
    }

    .title-text {
      white-space: normal;
      word-break: break-word;
      animation: titleShow var(--animation-duration) infinite cubic-bezier(.1, 0, .3, 1);

      &.is-once {
        animation: titleShowOnce var(--animation-duration) cubic-bezier(.1, 0, .3, 1) both;
      }
    }
  }

  .sub-title-box {
    overflow: hidden;
    display: inline-block;
    padding: 3px 7px;
    transform: translateY(-110%);
    animation: subContentBoxShow var(--animation-duration) infinite cubic-bezier(0.1, 0, 0.3, 1);

    &.is-once {
      animation: subContentBoxShowOnce var(--animation-duration) cubic-bezier(0.1, 0, 0.3, 1) both;
    }

    .sub-title-text {
      word-break: break-word;
    }
  }

  @keyframes titleBoxShow {
    0% {
      transform: var(--translate-value);
    }
    15% {
      transform: translateX(0);
    }
    80% {
      transform: translateX(0);
    }
    100% {
      transform: var(--translate-value);
    }
  }

  @keyframes titleBoxShowOnce {
    0% {
      transform: var(--translate-value);
    }
    15% {
      transform: translateX(0);
    }
    100% {
      transform: translateX(0);
    }
  }

  @keyframes titleShow {
    0% {
      transform: var(--translate-value);
    }
    10% {
      transform: var(--translate-value);
    }
    20% {
      transform: translateX(0);
    }
    80% {
      transform: translateX(0);
    }
    100% {
      transform: var(--translate-value);
    }
  }

  @keyframes titleShowOnce {
    0% {
      transform: var(--translate-value);
    }
    10% {
      transform: var(--translate-value);
    }
    20% {
      transform: translateX(0);
    }
    100% {
      transform: translateX(0);
    }
  }

  @keyframes subContentBoxShow {
    0% {
      transform: translateY(-110%);
    }
    15% {
      transform: translateY(-110%);
    }
    35% {
      transform: translateY(0);
    }
    80% {
      transform: translateY(0);
    }
    100% {
      transform: translateY(-110%);
    }
  }

  @keyframes subContentBoxShowOnce {
    0% {
      transform: translateY(-110%);
    }
    15% {
      transform: translateY(-110%);
    }
    35% {
      transform: translateY(0);
    }
    100% {
      transform: translateY(0);
    }
  }
</style>
