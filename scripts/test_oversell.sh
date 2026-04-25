#!/bin/bash

PRODUCT_ID=3
USER_ID=1
URL="http://127.0.0.1:5555/api/order/create"

echo "开始并发下单测试..."
echo "商品ID: ${PRODUCT_ID}"
echo "并发请求数: 10"

for i in {1..10}
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
echo "并发请求完成"
echo "请检查商品库存和订单数量"