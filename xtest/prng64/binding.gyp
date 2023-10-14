{
  "targets": [
    {
      "target_name": "nativeThrust",
      "sources": [
        "nativeThrust.cxx"
      ],
      "include_dirs": [
        '<!(node -e "require(\'nan\')")'
      ]
    }
  ]
}
