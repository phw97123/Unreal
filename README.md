# 언리얼 액션 포트폴리오

---

## 기능 구현 

Targeting, 무기 변경 (검, 활), 무기 콤보 및 무기 스킬 쿨타임, 캐릭터 IK, 플레이어 Dash, 에임 오프셋 등 

## 주요 기능 설명 

#### Dash 

![image](https://github.com/phw97123/Unreal/assets/132995834/1d3ae9aa-20fe-47fb-b7f4-a6c46c819b87)

벡터의 외적과 내적을 이용하여 플레이어가 지형의 경사면을 따라 Dash 하는 기능을 구현했습니다.

#### Targeting

![image](https://github.com/phw97123/Unreal/assets/132995834/c43c7693-7aca-4523-9710-7d6bc9e4b329)

화면 안에 Actor가 존재하는지 나타내는 함수로 일정 범위 안을 설정하는 ScreenRatio가 아닐 경우 화면 전체를 범위로 지정하여 사용합니다 

![image](https://github.com/phw97123/Unreal/assets/132995834/cadc181c-9c77-43f0-b97f-7769412883f2)

화면 중앙에서 일정 범위 밖으로 Target이 나갈 경우 화면의 일정 범위 안으로 Target이 들어오도록 카메라를 조정하는 기능을 구현했습니다 








