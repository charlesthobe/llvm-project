; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt -S -passes=instcombine < %s | FileCheck %s

define i32 @or_and_shifts1(i32 %x) {
; CHECK-LABEL: @or_and_shifts1(
; CHECK-NEXT:    [[I:%.*]] = shl i32 [[X:%.*]], 3
; CHECK-NEXT:    [[I1:%.*]] = and i32 [[I]], 8
; CHECK-NEXT:    [[I2:%.*]] = shl i32 [[X]], 5
; CHECK-NEXT:    [[I3:%.*]] = and i32 [[I2]], 32
; CHECK-NEXT:    [[I4:%.*]] = or i32 [[I1]], [[I3]]
; CHECK-NEXT:    ret i32 [[I4]]
;
  %i = shl i32 %x, 3
  %i1 = and i32 %i, 15
  %i2 = shl i32 %x, 5
  %i3 = and i32 %i2, 60
  %i4 = or i32 %i1, %i3
  ret i32 %i4
}

define i32 @or_and_shifts2(i32 %x) {
; CHECK-LABEL: @or_and_shifts2(
; CHECK-NEXT:    [[I:%.*]] = shl i32 [[X:%.*]], 3
; CHECK-NEXT:    [[I1:%.*]] = and i32 [[I]], 896
; CHECK-NEXT:    [[I2:%.*]] = lshr i32 [[X]], 4
; CHECK-NEXT:    [[I3:%.*]] = and i32 [[I2]], 7
; CHECK-NEXT:    [[I4:%.*]] = or i32 [[I1]], [[I3]]
; CHECK-NEXT:    ret i32 [[I4]]
;
  %i = shl i32 %x, 3
  %i1 = and i32 %i, 896
  %i2 = lshr i32 %x, 4
  %i3 = and i32 %i2, 7
  %i4 = or i32 %i1, %i3
  ret i32 %i4
}

define i32 @or_and_shift_shift_and(i32 %x) {
; CHECK-LABEL: @or_and_shift_shift_and(
; CHECK-NEXT:    [[I:%.*]] = shl i32 [[X:%.*]], 3
; CHECK-NEXT:    [[I1:%.*]] = and i32 [[I]], 56
; CHECK-NEXT:    [[I2:%.*]] = shl i32 [[X]], 2
; CHECK-NEXT:    [[I3:%.*]] = and i32 [[I2]], 28
; CHECK-NEXT:    [[I4:%.*]] = or i32 [[I1]], [[I3]]
; CHECK-NEXT:    ret i32 [[I4]]
;
  %i = and i32 %x, 7
  %i1 = shl i32 %i, 3
  %i2 = shl i32 %x, 2
  %i3 = and i32 %i2, 28
  %i4 = or i32 %i1, %i3
  ret i32 %i4
}

define i32 @multiuse1(i32 %x) {
; CHECK-LABEL: @multiuse1(
; CHECK-NEXT:    [[I21:%.*]] = shl i32 [[X:%.*]], 6
; CHECK-NEXT:    [[I6:%.*]] = and i32 [[I21]], 384
; CHECK-NEXT:    [[I32:%.*]] = lshr i32 [[X]], 1
; CHECK-NEXT:    [[I7:%.*]] = and i32 [[I32]], 3
; CHECK-NEXT:    [[I8:%.*]] = or i32 [[I7]], [[I6]]
; CHECK-NEXT:    ret i32 [[I8]]
;
  %i = and i32 %x, 2
  %i1 = and i32 %x, 4
  %i2 = shl nuw nsw i32 %i, 6
  %i3 = lshr exact i32 %i, 1
  %i4 = shl nuw nsw i32 %i1, 6
  %i5 = lshr exact i32 %i1, 1
  %i6 = or i32 %i2, %i4
  %i7 = or i32 %i3, %i5
  %i8 = or i32 %i7, %i6
  ret i32 %i8
}

define i32 @multiuse2(i32 %x) {
; CHECK-LABEL: @multiuse2(
; CHECK-NEXT:    [[I:%.*]] = shl i32 [[X:%.*]], 1
; CHECK-NEXT:    [[I2:%.*]] = and i32 [[I]], 12
; CHECK-NEXT:    [[I6:%.*]] = shl i32 [[X]], 8
; CHECK-NEXT:    [[I7:%.*]] = and i32 [[I6]], 24576
; CHECK-NEXT:    [[I14:%.*]] = shl i32 [[X]], 8
; CHECK-NEXT:    [[I9:%.*]] = and i32 [[I14]], 7680
; CHECK-NEXT:    [[I10:%.*]] = or i32 [[I7]], [[I9]]
; CHECK-NEXT:    [[I85:%.*]] = shl i32 [[X]], 1
; CHECK-NEXT:    [[I11:%.*]] = and i32 [[I85]], 240
; CHECK-NEXT:    [[I12:%.*]] = or i32 [[I2]], [[I11]]
; CHECK-NEXT:    [[I13:%.*]] = or i32 [[I10]], [[I12]]
; CHECK-NEXT:    ret i32 [[I13]]
;
  %i = and i32 %x, 6
  %i1 = shl nuw nsw i32 %i, 8
  %i2 = shl nuw nsw i32 %i, 1
  %i3 = and i32 %x, 24
  %i4 = shl nuw nsw i32 %i3, 8
  %i5 = shl nuw nsw i32 %i3, 1
  %i6 = and i32 %x, 96
  %i7 = shl nuw nsw i32 %i6, 8
  %i8 = shl nuw nsw i32 %i6, 1
  %i9 = or i32 %i1, %i4
  %i10 = or i32 %i7, %i9
  %i11 = or i32 %i8, %i5
  %i12 = or i32 %i2, %i11
  %i13 = or i32 %i10, %i12
  ret i32 %i13
}

define i32 @multiuse3(i32 %x) {
; CHECK-LABEL: @multiuse3(
; CHECK-NEXT:    [[I:%.*]] = and i32 [[X:%.*]], 96
; CHECK-NEXT:    [[I1:%.*]] = shl nuw nsw i32 [[I]], 6
; CHECK-NEXT:    [[I2:%.*]] = lshr exact i32 [[I]], 1
; CHECK-NEXT:    [[I3:%.*]] = shl i32 [[X]], 6
; CHECK-NEXT:    [[I4:%.*]] = and i32 [[I3]], 1920
; CHECK-NEXT:    [[I5:%.*]] = or i32 [[I1]], [[I4]]
; CHECK-NEXT:    [[I6:%.*]] = lshr i32 [[X]], 1
; CHECK-NEXT:    [[I7:%.*]] = and i32 [[I6]], 15
; CHECK-NEXT:    [[I8:%.*]] = or i32 [[I2]], [[I7]]
; CHECK-NEXT:    [[I9:%.*]] = or i32 [[I8]], [[I5]]
; CHECK-NEXT:    ret i32 [[I9]]
;
  %i = and i32 %x, 96
  %i1 = shl nuw nsw i32 %i, 6
  %i2 = lshr exact i32 %i, 1
  %i3 = shl i32 %x, 6
  %i4 = and i32 %i3, 1920
  %i5 = or i32 %i1, %i4
  %i6 = lshr i32 %x, 1
  %i7 = and i32 %i6, 15
  %i8 = or i32 %i2, %i7
  %i9 = or i32 %i8, %i5
  ret i32 %i9
}

define i32 @multiuse4(i32 %x) local_unnamed_addr {
; CHECK-LABEL: @multiuse4(
; CHECK-NEXT:    [[I:%.*]] = and i32 [[X:%.*]], 100663296
; CHECK-NEXT:    [[I1:%.*]] = icmp sgt i32 [[X]], -1
; CHECK-NEXT:    br i1 [[I1]], label [[IF:%.*]], label [[ELSE:%.*]]
; CHECK:       if:
; CHECK-NEXT:    [[I2:%.*]] = lshr exact i32 [[I]], 22
; CHECK-NEXT:    [[I3:%.*]] = lshr i32 [[X]], 22
; CHECK-NEXT:    [[I4:%.*]] = and i32 [[I3]], 480
; CHECK-NEXT:    [[I5:%.*]] = or i32 [[I4]], [[I2]]
; CHECK-NEXT:    br label [[END:%.*]]
; CHECK:       else:
; CHECK-NEXT:    [[I6:%.*]] = lshr exact i32 [[I]], 17
; CHECK-NEXT:    [[I7:%.*]] = lshr i32 [[X]], 17
; CHECK-NEXT:    [[I8:%.*]] = and i32 [[I7]], 15360
; CHECK-NEXT:    [[I9:%.*]] = or i32 [[I8]], [[I6]]
; CHECK-NEXT:    br label [[END]]
; CHECK:       end:
; CHECK-NEXT:    [[I10:%.*]] = phi i32 [ [[I5]], [[IF]] ], [ [[I9]], [[ELSE]] ]
; CHECK-NEXT:    ret i32 [[I10]]
;
  %i = and i32 %x, 100663296
  %i1 = icmp sgt i32 %x, -1
  br i1 %i1, label %if, label %else

if:
  %i2 = lshr exact i32 %i, 22
  %i3 = lshr i32 %x, 22
  %i4 = and i32 %i3, 480
  %i5 = or i32 %i4, %i2
  br label %end

else:
  %i6 = lshr exact i32 %i, 17
  %i7 = lshr i32 %x, 17
  %i8 = and i32 %i7, 15360
  %i9 = or i32 %i8, %i6
  br label %end

end:
  %i10 = phi i32 [ %i5, %if ], [ %i9, %else ]
  ret i32 %i10
}

define i32 @multiuse5(i32 %x) local_unnamed_addr {
; CHECK-LABEL: @multiuse5(
; CHECK-NEXT:    [[I:%.*]] = shl i32 [[X:%.*]], 5
; CHECK-NEXT:    [[I1:%.*]] = icmp sgt i32 [[X]], -1
; CHECK-NEXT:    br i1 [[I1]], label [[IF:%.*]], label [[ELSE:%.*]]
; CHECK:       if:
; CHECK-NEXT:    [[I2:%.*]] = and i32 [[I]], 21760
; CHECK-NEXT:    [[I3:%.*]] = shl i32 [[X]], 5
; CHECK-NEXT:    [[I4:%.*]] = and i32 [[I3]], 43520
; CHECK-NEXT:    [[I5:%.*]] = or i32 [[I4]], [[I2]]
; CHECK-NEXT:    br label [[END:%.*]]
; CHECK:       else:
; CHECK-NEXT:    [[I6:%.*]] = and i32 [[I]], 5570560
; CHECK-NEXT:    [[I7:%.*]] = shl i32 [[X]], 5
; CHECK-NEXT:    [[I8:%.*]] = and i32 [[I7]], 11141120
; CHECK-NEXT:    [[I9:%.*]] = or i32 [[I8]], [[I6]]
; CHECK-NEXT:    br label [[END]]
; CHECK:       end:
; CHECK-NEXT:    [[I10:%.*]] = phi i32 [ [[I5]], [[IF]] ], [ [[I9]], [[ELSE]] ]
; CHECK-NEXT:    ret i32 [[I10]]
;
  %i = shl i32 %x, 5
  %i1 = icmp sgt i32 %x, -1
  br i1 %i1, label %if, label %else

if:
  %i2 = and i32 %i, 21760
  %i3 = and i32 %x, 1360
  %i4 = shl nuw nsw i32 %i3, 5
  %i5 = or i32 %i4, %i2
  br label %end

else:
  %i6 = and i32 %i, 5570560
  %i7 = and i32 %x, 348160
  %i8 = shl nuw nsw i32 %i7, 5
  %i9 = or i32 %i8, %i6
  br label %end

end:
  %i10 = phi i32 [ %i5, %if ], [ %i9, %else ]
  ret i32 %i10
}

define i32 @shl_mask(i32 %x) {
; CHECK-LABEL: @shl_mask(
; CHECK-NEXT:    [[Z:%.*]] = and i32 [[X:%.*]], 255
; CHECK-NEXT:    [[S:%.*]] = shl nuw nsw i32 [[Z]], 8
; CHECK-NEXT:    [[R:%.*]] = or i32 [[Z]], [[S]]
; CHECK-NEXT:    ret i32 [[R]]
;
  %z = and i32 %x, 255
  %s = shl i32 %z, 8
  %r = or i32 %z, %s
  ret i32 %r
}

define i32 @shl_mask_wrong_shl_const(i32 %x) {
; CHECK-LABEL: @shl_mask_wrong_shl_const(
; CHECK-NEXT:    [[Z:%.*]] = and i32 [[X:%.*]], 255
; CHECK-NEXT:    [[S:%.*]] = shl nuw nsw i32 [[Z]], 7
; CHECK-NEXT:    [[R:%.*]] = or i32 [[Z]], [[S]]
; CHECK-NEXT:    ret i32 [[R]]
;
  %z = and i32 %x, 255
  %s = shl i32 %z, 7
  %r = or i32 %z, %s
  ret i32 %r
}

define i37 @shl_mask_weird_type(i37 %x) {
; CHECK-LABEL: @shl_mask_weird_type(
; CHECK-NEXT:    [[Z:%.*]] = and i37 [[X:%.*]], 255
; CHECK-NEXT:    [[S:%.*]] = shl nuw nsw i37 [[Z]], 8
; CHECK-NEXT:    [[R:%.*]] = or i37 [[Z]], [[S]]
; CHECK-NEXT:    ret i37 [[R]]
;
  %z = and i37 %x, 255
  %s = shl i37 %z, 8
  %r = or i37 %z, %s
  ret i37 %r
}

define i32 @shl_mask_extra_use(i32 %x, ptr %p) {
; CHECK-LABEL: @shl_mask_extra_use(
; CHECK-NEXT:    [[Z:%.*]] = and i32 [[X:%.*]], 255
; CHECK-NEXT:    [[S:%.*]] = shl nuw nsw i32 [[Z]], 8
; CHECK-NEXT:    store i32 [[S]], ptr [[P:%.*]], align 4
; CHECK-NEXT:    [[R:%.*]] = or i32 [[Z]], [[S]]
; CHECK-NEXT:    ret i32 [[R]]
;
  %z = and i32 %x, 255
  %s = shl i32 %z, 8
  store i32 %s, ptr %p, align 4
  %r = or i32 %z, %s
  ret i32 %r
}

; This could be "Z * 65793".

define i32 @shl_mul_mask(i32 %x) {
; CHECK-LABEL: @shl_mul_mask(
; CHECK-NEXT:    [[Z:%.*]] = and i32 [[X:%.*]], 255
; CHECK-NEXT:    [[M:%.*]] = mul nuw nsw i32 [[Z]], 65537
; CHECK-NEXT:    [[S:%.*]] = shl nuw nsw i32 [[Z]], 8
; CHECK-NEXT:    [[R:%.*]] = or i32 [[M]], [[S]]
; CHECK-NEXT:    ret i32 [[R]]
;
  %z = and i32 %x, 255
  %m = mul i32 %z, 65537
  %s = shl i32 %z, 8
  %r = or i32 %m, %s
  ret i32 %r
}

define i32 @shl_mul_mask_wrong_mul_const(i32 %x) {
; CHECK-LABEL: @shl_mul_mask_wrong_mul_const(
; CHECK-NEXT:    [[Z:%.*]] = and i32 [[X:%.*]], 255
; CHECK-NEXT:    [[M:%.*]] = mul nuw nsw i32 [[Z]], 65535
; CHECK-NEXT:    [[S:%.*]] = shl nuw nsw i32 [[Z]], 8
; CHECK-NEXT:    [[R:%.*]] = or i32 [[M]], [[S]]
; CHECK-NEXT:    ret i32 [[R]]
;
  %z = and i32 %x, 255
  %m = mul i32 %z, 65535
  %s = shl i32 %z, 8
  %r = or i32 %m, %s
  ret i32 %r
}
