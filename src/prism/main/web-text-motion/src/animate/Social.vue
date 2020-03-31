<template>
  <div ref="$container" @animationend="animationend" class="box-animate" :class="className">
    <div :style="style" class="line-box">
      <div ref="$icon" :style="{'--icon-speed':iconSpeed,...iconStyle}" class="type-icon">
        <img :src="src" style="height: 100%;width: 100%;">
      </div>
      <div ref="$text" class="content-box" :style="{'--box-max-width':boxMaxWidth}">
        <div :style="{'--content-speed':durationNum+'ms'}" class="animate-content wrap-center">
          <div ref="$content" :style="{'textAlign':data.align}" class="bg-content">
            <div class="min-height-row pre-wrap" v-for="(item,index) in data.content" :key="index">{{item}}</div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import iconNavertv from '../assets/icon-navertv.png'
import iconYoutube from '../assets/icon-youtube.png'
import iconTwitch from '../assets/icon-twich.png'
import iconAfreeca from '../assets/icon-afreeca.png'
import iconIns from '../assets/icon-ins.png'
import iconFacebook from '../assets/icon-facebook.png'
import iconVlive from '../assets/icon-vlive.png'

export default {
  name: 'SocialShow',
  props: ['data'],
  data () {
    return {
      className: '',
      style: {},
      boxMaxWidth: 'calc(100% - 50px)',
      iconStyle: {
        width: '32px',
        height: '32px'
      },
      images: {
        'Social_2': iconNavertv,
        'Social_3': iconYoutube,
        'Social_4': iconTwitch,
        'Social_5': iconAfreeca,
        'Social_1': iconVlive,
        'Social_6': iconIns,
        'Social_7': iconFacebook
      }
    }
  },
  computed: {
    durationNum () {
      let defaultDuration = 1000
      return defaultDuration * this.data.animateRate
    },
    iconSpeed () {
      return this.durationNum * 0.3 + 'ms'
    },
    src () {
      return this.images[this.data.type]
    }
  },
  mounted () {
    this.className = 'animation-in'
    let width = parseInt(getComputedStyle(this.$refs.$content).fontSize) * 3 / 2 + 'px'
    this.iconStyle = {
      height: width,
      minWidth: width,
      width: width
    }
    this.boxMaxWidth = `calc(100% - ${this.iconStyle.width})`
    this.style = {
      'background': this.data.background,
      '--offset-x': (this.$refs.$icon.offsetWidth + this.$refs.$text.offsetWidth) / 2 + 'px',
      '--content-speed': this.durationNum + 'ms'
    }
  },
  methods: {
    animationend (el) {
      let classList = this.$refs.$container.classList
      if (classList.contains('animation-in')) {
        this.className = 'animation-in-next'
      } else if (classList.contains('animation-in-next')) {
        this.animationInEnd()
      } else if (classList.contains('animation-out')) {
        this.animationOutEnd()
      }
    },
    animationInEnd () {
      if (!this.data.isOnce) {
        setTimeout(() => {
          this.className = 'animation-out'
        }, 2000)
      }
    },
    animationOutEnd () {
      setTimeout(() => {
        this.className = 'animation-in'
      }, 100)
    }
  }
}
</script>

<style scoped lang="scss">
  .content-box {
    overflow: hidden;
    max-width: var(--box-max-width);
  }

  .line-box {
    padding: 5px;
    --offset-x: 0px;
    overflow: hidden;
    display: flex;
    align-items: flex-start;
    justify-content: center;
    transform: translateX(var(--offset-x));
  }

  .type-icon {
    z-index: 2;
    display: flex;
    align-items: center;
    margin-right: 8px;
  }

  .animate-content {
    transform: translate3d(-110%, 0, 0);
  }

  .box-animate {
    display: flex;
    justify-content: center;
    width: 100%;
    z-index: 1;

    &.animation-in {
      .type-icon {
        --icon-speed: 300ms;
        animation-name: iconExpand;
        animation-duration: var(--icon-speed);
        animation-timing-function: ease;
        animation-fill-mode: both;
      }
    }

    &.animation-in-next {
      .line-box {
        --content-speed: 1000ms;
        animation-name: BoxSlide;
        animation-duration: var(--content-speed);
        animation-timing-function: cubic-bezier(0, 0, 0.15, 1);
        animation-fill-mode: both;
      }

      .animate-content {
        --content-speed: 1000ms;
        animation-name: ContentSlide;
        animation-duration: var(--content-speed);
        animation-timing-function: cubic-bezier(0.33, 0, 0.15, 1);
        animation-fill-mode: both;
      }
    }

    &.animation-out {
      .line-box {
        transform: translateX(0);
        animation-name: fadeOut;
        animation-duration: .2s;
        animation-timing-function: ease;
        animation-fill-mode: both;
      }

      .animate-content {
        visibility: visible;
        transform: translate3d(0, 0, 0);
        animation-fill-mode: both;
      }
    }
  }

  @keyframes iconExpand {
    0% {
      transform: scale(0, 0);
    }
    100% {
      transform: scale(1, 1);
    }
  }

  @keyframes BoxSlide {
    0% {
      transform: translateX(var(--offset-x));
    }
    100% {
      transform: translateX(0);
    }
  }

  @keyframes ContentSlide {
    0% {
      visibility: hidden;
      transform: translate3d(-110%, 0, 0);
    }
    100% {
      visibility: visible;
      transform: translate3d(0, 0, 0);
    }
  }
</style>
