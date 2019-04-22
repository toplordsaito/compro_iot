<template>
  <div @click="handleClick" class="control-card col-6 p-1">
    <div :class="['row wrapper', {'isOff': !isOn}]">
      <div class="col-4">
        <img class="w-100" :src="image" alt="">
      </div>
      <div class="col-8 center">
        <p>
          {{ name }} is 
          <b v-if="isOn">on</b>
          <b v-else>off</b>
        </p>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'ControlCard',
  props: ['name', 'isOn', 'image'],
  methods: {
    handleClick () {
      this.$emit('isClick')
    }
  }
}
</script>

<style lang="scss" scoped>
.control-card {
  .wrapper {
    padding: 10px 3px;
    background: rgb(230, 230, 230);
    margin: 1px;
  }
  p {
    margin-bottom: 0;
  }
}
.center {
  display: flex;
  justify-content: center;
  align-items: center;
}
.isOff {
  background: #777777!important;
  color: #fff;
}
</style>

