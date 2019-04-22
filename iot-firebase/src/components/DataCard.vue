<template>
  <div class="col-4">
    <h5>{{ name }}</h5>
    <h2>{{ value }}</h2>
  </div>
</template>

<script>
export default {
  name: 'DataCard',
  props: ['name', 'value']
}
</script>
