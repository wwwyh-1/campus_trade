#!/bin/bash

PRODUCT_ID=4
USER_ID=1
URL="http://127.0.0.1:5555/api/seckill/create"

echo "开始秒杀并发测试..."
echo "商品ID: ${PRODUCT_ID}"
echo "并发请求数: 20"

for i in {1..20}
do
  curl -s -X POST ${URL} \
  -H "Content-Type: application/json" \
  -d "{
    \"user_id\": ${USER_ID},
    \"product_id\": ${PRODUCT_ID}
  }" &
done

wait

echo ""
echo "秒杀并发请求完成"
echo "请检查 Redis 库存、MySQL 商品库存和订单数量"